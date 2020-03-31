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
#include "vkbinputlayoutattached.h"

#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbInputPopup::VkbInputPopup(QObject *parent)
    : QQuickPopup(parent)
{
}

void VkbInputPopup::setVisible(bool visible)
{
    QQuickPopup::setVisible(visible);
    if (visible)
        popupItem()->grabMouse();
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
}

void VkbInputPopup::mouseMoveEvent(QMouseEvent *event)
{
    QQuickPopup::mouseMoveEvent(event);
    updateCurrentButton(buttonAt(contentItem(), event->globalPos()));
}

void VkbInputPopup::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickPopup::mouseReleaseEvent(event);
    if (m_currentButton) {
        VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(m_currentButton);
        if (attached)
            emit keySelected(attached->inputKey());
    }
    close();
}

void VkbInputPopup::mouseUngrabEvent()
{
    QQuickPopup::mouseUngrabEvent();
    close();
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
