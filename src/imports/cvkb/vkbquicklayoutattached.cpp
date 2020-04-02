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

#include "vkbquicklayoutattached.h"

VkbQuickLayoutAttached::VkbQuickLayoutAttached(QObject *parent) : QObject(parent)
{
}

Qt::Key VkbQuickLayoutAttached::key() const
{
    return m_key.key;
}

QString VkbQuickLayoutAttached::text() const
{
    return m_key.text;
}

QStringList VkbQuickLayoutAttached::alt() const
{
    return m_key.alt;
}

bool VkbQuickLayoutAttached::autoRepeat() const
{
    return m_key.autoRepeat;
}

bool VkbQuickLayoutAttached::isCheckable() const
{
    return m_key.checkable;
}

bool VkbQuickLayoutAttached::isChecked() const
{
    return m_key.checked;
}

VkbInputKey VkbQuickLayoutAttached::inputKey() const
{
    return m_key;
}

void VkbQuickLayoutAttached::setInputKey(const VkbInputKey &key)
{
    m_key = key;
}

VkbQuickLayoutAttached *VkbQuickLayoutAttached::qmlAttachedProperties(QObject *object)
{
    return new VkbQuickLayoutAttached(object);
}

VkbQuickLayoutAttached *VkbQuickLayoutAttached::qmlAttachedPropertiesObject(QObject *object)
{
    if (!object)
        return nullptr;

    return qobject_cast<VkbQuickLayoutAttached *>(::qmlAttachedPropertiesObject<VkbQuickLayoutAttached>(object));
}
