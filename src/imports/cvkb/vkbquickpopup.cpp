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

#include "vkbquickpopup.h"
#include "vkbquickdelegate.h"
#include "vkbquicklayoutattached.h"
#include "vkbquicklayout.h"
#include "vkbquickmodel.h"

#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbQuickPopup::VkbQuickPopup(QObject *parent)
    : QQuickPopup(parent),
      m_model(new VkbQuickModel(this)),
      m_layout(new VkbQuickLayout(popupItem()))
{
    setClosePolicy(CloseOnEscape);
    setContentItem(m_layout);

    connect(this, &QQuickPopup::closed, this, &QObject::deleteLater);
    connect(m_model, &VkbQuickModel::delegatesChanged, this, &VkbQuickPopup::updateButtons);

    // ### TODO: fix QQuickPopup::spacingChange()
    if (QQuickControl *control = qobject_cast<QQuickControl *>(popupItem()))
        connect(control, &QQuickControl::spacingChanged, this, &VkbQuickPopup::updateSpacing);
}

QStringList VkbQuickPopup::alt() const
{
    return m_alt;
}

void VkbQuickPopup::setAlt(const QStringList &alt)
{
    if (m_alt == alt)
        return;

    m_alt = alt;
    emit altChanged();
}

QQmlListProperty<VkbQuickDelegate> VkbQuickPopup::delegates()
{
    return m_model->delegates();
}

void VkbQuickPopup::setVisible(bool visible)
{
    QQuickPopup::setVisible(visible);
    if (visible)
        popupItem()->grabMouse();
}

void VkbQuickPopup::componentComplete()
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

void VkbQuickPopup::mousePressEvent(QMouseEvent *event)
{
    QQuickPopup::mousePressEvent(event);
    updateCurrentButton(buttonAt(contentItem(), event->globalPos()));
    handlePress(m_currentButton);
}

void VkbQuickPopup::mouseMoveEvent(QMouseEvent *event)
{
    QQuickPopup::mouseMoveEvent(event);
    QQuickAbstractButton *oldCurrent = m_currentButton;
    updateCurrentButton(buttonAt(contentItem(), event->globalPos()));
    if (m_currentButton != oldCurrent) {
        handleCancel(oldCurrent);
        handlePress(m_currentButton);
    }
}

void VkbQuickPopup::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickPopup::mouseReleaseEvent(event);
    handleRelease(m_currentButton);
    updateCurrentButton(nullptr);
    close();
}

void VkbQuickPopup::mouseUngrabEvent()
{
    QQuickPopup::mouseUngrabEvent();
    handleCancel(m_currentButton);
    updateCurrentButton(nullptr);
    close();
}

void VkbQuickPopup::handlePress(QQuickAbstractButton *button)
{
    VkbQuickLayoutAttached *attached = VkbQuickLayoutAttached::qmlAttachedPropertiesObject(button);
    if (attached)
        emit keyPressed(attached->inputKey());
}

void VkbQuickPopup::handleRelease(QQuickAbstractButton *button)
{
    VkbQuickLayoutAttached *attached = VkbQuickLayoutAttached::qmlAttachedPropertiesObject(button);
    if (attached)
        emit keyReleased(attached->inputKey());
}

void VkbQuickPopup::handleCancel(QQuickAbstractButton *button)
{
    VkbQuickLayoutAttached *attached = VkbQuickLayoutAttached::qmlAttachedPropertiesObject(button);
    if (attached)
        emit keyCanceled(attached->inputKey());
}

void VkbQuickPopup::updateSpacing()
{
    m_layout->setSpacing(spacing());
}

void VkbQuickPopup::updateButtons()
{
    if (!isComponentComplete())
        return;

    QHash<VkbInputKey, QQuickAbstractButton *> newButtons;
    QHash<VkbInputKey, QQuickAbstractButton *> oldButtons = m_layout->buttons();

    QVector<VkbInputKey> row;
    for (const QString &text : qAsConst(m_alt)) {
        VkbInputKey key(text);
        QQuickAbstractButton *button = oldButtons.take(key);
        if (!button)
            button = m_model->createButton(key, m_layout);
        newButtons.insert(key, button);
        row += key;
    }

    for (QQuickAbstractButton *button : qAsConst(oldButtons))
        button->deleteLater();

    m_layout->setLayout(VkbInputLayout(row));
    m_layout->setButtons(newButtons);
}

void VkbQuickPopup::updateCurrentButton(QQuickAbstractButton *button)
{
    if (m_currentButton == button)
        return;

    if (m_currentButton)
        m_currentButton->resetDown();
    if (button)
        button->setDown(true);

    m_currentButton = button;
}
