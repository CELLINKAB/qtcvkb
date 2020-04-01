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

#include "vkbinputpanel.h"
#include "vkbinputdelegate.h"
#include "vkbinputlayoutattached.h"
#include "vkbinputlayoutitem.h"
#include "vkbinputmodel.h"
#include "vkbinputpopup.h"

#include <QtQuickTemplates2/private/qquickcontrol_p.h>
#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbInputPanel::VkbInputPanel(QObject *parent)
    : QQuickPopup(parent),
      m_model(new VkbInputModel(this)),
      m_layoutItem(new VkbInputLayoutItem(popupItem()))
{
    setClosePolicy(CloseOnEscape);
    setContentItem(m_layoutItem);

    connect(m_model, &VkbInputModel::delegatesChanged, this, &VkbInputPanel::updateButtons);

    connect(this, &QQuickPopup::visibleChanged, this, &VkbInputPanel::visibleChanged);
    connect(this, &QQuickPopup::openedChanged, this, &VkbInputPanel::animatingChanged);
    connect(this, &QQuickPopup::visibleChanged, this, &VkbInputPanel::animatingChanged);
    connect(this, &QQuickPopup::localeChanged, this, &VkbInputPanel::localeChanged);

    // ### TODO: fix QQuickPopup::spacingChange()
    if (QQuickControl *control = qobject_cast<QQuickControl *>(popupItem()))
        connect(control, &QQuickControl::spacingChanged, this, &VkbInputPanel::updateSpacing);
}

bool VkbInputPanel::isVisible() const
{
    return QQuickPopup::isVisible();
}

void VkbInputPanel::setVisible(bool visible)
{
    QQuickPopup::setVisible(visible);
}

bool VkbInputPanel::isAnimating() const
{
    return isVisible() && !isOpened();
}

QRectF VkbInputPanel::rect() const
{
    return m_rect;
}

QLocale VkbInputPanel::locale() const
{
    return QQuickPopup::locale();
}

Qt::LayoutDirection VkbInputPanel::inputDirection() const
{
    return Qt::LeftToRight;
}

VkbInputLayout VkbInputPanel::layout() const
{
    return m_layoutItem->layout();
}

void VkbInputPanel::setLayout(const VkbInputLayout &layout)
{
    if (m_layoutItem->layout() == layout)
        return;

    m_layoutItem->setLayout(layout);
    updateButtons();
    emit layoutChanged();
}

QQmlListProperty<VkbInputDelegate> VkbInputPanel::delegates()
{
    return m_model->delegates();
}

void VkbInputPanel::handleKeyPress()
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyPressed(attached->inputKey());
}

void VkbInputPanel::handleKeyRelease()
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyReleased(attached->inputKey());
}

void VkbInputPanel::handleKeyCancel()
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyCanceled(attached->inputKey());
}

void VkbInputPanel::handleKeyPressAndHold()
{
    QQuickAbstractButton *button = qobject_cast<QQuickAbstractButton *>(sender());
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(button);
    if (!attached)
        return;

    if (!attached->alt().isEmpty()) {
        VkbInputPopup *popup = m_model->createPopup(attached->inputKey(), button);
        if (!popup)
            return;

        popup->open();
        connect(popup, &VkbInputPopup::keyPressed, this, &VkbInputPanel::keyPressed);
        connect(popup, &VkbInputPopup::keyReleased, this, &VkbInputPanel::keyReleased);
        connect(popup, &VkbInputPopup::keyCanceled, this, &VkbInputPanel::keyCanceled);
    }

    emit keyPressAndHold(attached->inputKey());
}

void VkbInputPanel::componentComplete()
{
    QQuickPopup::componentComplete();
    updateButtons();
}

void VkbInputPanel::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPopup::geometryChanged(newGeometry, oldGeometry);
    updateRect(newGeometry);
}

void VkbInputPanel::updateRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    m_rect = rect;
    emit rectChanged();
}

void VkbInputPanel::updateSpacing()
{
    m_layoutItem->setSpacing(spacing());
}

void VkbInputPanel::updateButtons()
{
    if (!isComponentComplete())
        return;

    QHash<VkbInputKey, QQuickAbstractButton *> newButtons;
    QHash<VkbInputKey, QQuickAbstractButton *> oldButtons = m_layoutItem->buttons();

    VkbInputLayout layout = m_layoutItem->layout();
    for (int r = 0; r < layout.rowCount(); ++r) {
        const QVector<VkbInputKey> row = layout.rowAt(r);
        for (int c = 0; c < row.count(); ++c) {
            const VkbInputKey &key = row.at(c);
            QQuickAbstractButton *button = oldButtons.take(key);
            if (!button) {
                button = m_model->createButton(key, m_layoutItem);
                connect(button, &QQuickAbstractButton::pressed, this, &VkbInputPanel::handleKeyPress);
                connect(button, &QQuickAbstractButton::released, this, &VkbInputPanel::handleKeyRelease);
                connect(button, &QQuickAbstractButton::canceled, this, &VkbInputPanel::handleKeyCancel);
                connect(button, &QQuickAbstractButton::pressAndHold, this, &VkbInputPanel::handleKeyPressAndHold);
            }
            newButtons.insert(key, button);
        }
    }

    for (QQuickAbstractButton *button : qAsConst(oldButtons))
        button->deleteLater();

    m_layoutItem->setButtons(newButtons);
}
