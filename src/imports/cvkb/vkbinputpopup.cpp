/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2020 CELLINK AB <info@cellink.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "vkbinputpopup.h"
#include "vkbinputdelegate.h"
#include "vkbinputlayoutattached.h"
#include "vkbinputlayoutitem.h"
#include "vkbinputmodel.h"

#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbInputPopup::VkbInputPopup(QObject *parent)
    : QQuickPopup(parent),
      m_model(new VkbInputModel(this)),
      m_layoutItem(new VkbInputLayoutItem(popupItem()))
{
    setClosePolicy(CloseOnEscape);
    setContentItem(m_layoutItem);

    connect(this, &QQuickPopup::closed, this, &QObject::deleteLater);
    connect(m_model, &VkbInputModel::delegatesChanged, this, &VkbInputPopup::updateButtons);

    // ### TODO: fix QQuickPopup::spacingChange()
    if (QQuickControl *control = qobject_cast<QQuickControl *>(popupItem()))
        connect(control, &QQuickControl::spacingChanged, this, &VkbInputPopup::updateSpacing);
}

QStringList VkbInputPopup::alt() const
{
    return m_alt;
}

void VkbInputPopup::setAlt(const QStringList &alt)
{
    if (m_alt == alt)
        return;

    m_alt = alt;
    emit altChanged();
}

QQmlListProperty<VkbInputDelegate> VkbInputPopup::delegates()
{
    return m_model->delegates();
}

void VkbInputPopup::setVisible(bool visible)
{
    QQuickPopup::setVisible(visible);
    if (visible)
        popupItem()->grabMouse();
}

void VkbInputPopup::componentComplete()
{
    QQuickPopup::componentComplete();
    updateButtons();
}

QQuickItem *childAt(QQuickItem *parent, const QPointF &globalPos)
{
    QPointF pos = parent->mapFromGlobal(globalPos);
    return parent->childAt(pos.x(), pos.y());
}

QQuickAbstractButton *buttonAt(QQuickItem *parent, const QPointF &globalPos)
{
    QQuickItem *child = childAt(parent, globalPos);
    while (child) {
        QQuickAbstractButton *button = qobject_cast<QQuickAbstractButton *>(child);
        if (button)
            return button;

        child = childAt(child, globalPos);
    }
    return nullptr;
}

void VkbInputPopup::mousePressEvent(QMouseEvent *event)
{
    QQuickPopup::mousePressEvent(event);
    updateCurrentButton(buttonAt(contentItem(), event->globalPos()));
    handlePress(m_currentButton);
}

void VkbInputPopup::mouseMoveEvent(QMouseEvent *event)
{
    QQuickPopup::mouseMoveEvent(event);
    QQuickAbstractButton *oldCurrent = m_currentButton;
    updateCurrentButton(buttonAt(contentItem(), event->globalPos()));
    if (m_currentButton != oldCurrent) {
        handleCancel(oldCurrent);
        handlePress(m_currentButton);
    }
}

void VkbInputPopup::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickPopup::mouseReleaseEvent(event);
    handleRelease(m_currentButton);
    updateCurrentButton(nullptr);
    close();
}

void VkbInputPopup::mouseUngrabEvent()
{
    QQuickPopup::mouseUngrabEvent();
    handleCancel(m_currentButton);
    updateCurrentButton(nullptr);
    close();
}

void VkbInputPopup::handlePress(QQuickAbstractButton *button)
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(button);
    if (attached)
        emit keyPressed(attached->inputKey());
}

void VkbInputPopup::handleRelease(QQuickAbstractButton *button)
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(button);
    if (attached)
        emit keyReleased(attached->inputKey());
}

void VkbInputPopup::handleCancel(QQuickAbstractButton *button)
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(button);
    if (attached)
        emit keyCanceled(attached->inputKey());
}

void VkbInputPopup::updateSpacing()
{
    m_layoutItem->setSpacing(spacing());
}

void VkbInputPopup::updateButtons()
{
    if (!isComponentComplete())
        return;

    QHash<VkbInputKey, QQuickAbstractButton *> newButtons;
    QHash<VkbInputKey, QQuickAbstractButton *> oldButtons = m_layoutItem->buttons();

    QVector<VkbInputKey> row;
    for (const QString &text : qAsConst(m_alt)) {
        VkbInputKey key(text);
        QQuickAbstractButton *button = oldButtons.take(key);
        if (!button)
            button = m_model->createButton(key, m_layoutItem);
        newButtons.insert(key, button);
        row += key;
    }

    for (QQuickAbstractButton *button : qAsConst(oldButtons))
        button->deleteLater();

    m_layoutItem->setLayout(VkbInputLayout(row));
    m_layoutItem->setButtons(newButtons);
}

void VkbInputPopup::updateCurrentButton(QQuickAbstractButton *button)
{
    if (m_currentButton == button)
        return;

    if (m_currentButton)
        m_currentButton->resetDown();
    if (button)
        button->setDown(true);

    m_currentButton = button;
}
