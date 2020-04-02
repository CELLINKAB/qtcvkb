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

#include "vkbquickdelegate.h"

VkbQuickDelegate::VkbQuickDelegate(QObject *parent) : QObject(parent)
{
}

Qt::Key VkbQuickDelegate::key() const
{
    return m_key;
}

void VkbQuickDelegate::setKey(Qt::Key key)
{
    if (m_key == key)
        return;

    m_key = key;
    emit keyChanged();
}

QQmlComponent *VkbQuickDelegate::button() const
{
    return m_button;
}

void VkbQuickDelegate::setButton(QQmlComponent *button)
{
    if (m_button == button)
        return;

    m_button = button;
    emit buttonChanged();
}

QQmlComponent *VkbQuickDelegate::popup() const
{
    return m_popup;
}

void VkbQuickDelegate::setPopup(QQmlComponent *popup)
{
    if (m_popup == popup)
        return;

    m_popup = popup;
    emit popupChanged();
}
