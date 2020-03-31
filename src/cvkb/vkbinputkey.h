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

#ifndef VKBINPUTKEY_H
#define VKBINPUTKEY_H

#include <QtCVkb/vkbglobal.h>
#include <QtCore/qobjectdefs.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>

class Q_CVKB_EXPORT VkbInputKey
{
    Q_GADGET
    Q_PROPERTY(Qt::Key key MEMBER key)
    Q_PROPERTY(QString text MEMBER text)
    Q_PROPERTY(QStringList alt MEMBER alt)
    Q_PROPERTY(qreal span MEMBER span)
    Q_PROPERTY(bool autoRepeat MEMBER autoRepeat)
    Q_PROPERTY(bool checkable MEMBER checkable)
    Q_PROPERTY(bool checked MEMBER checked)

public:
    inline bool operator==(const VkbInputKey &other) const;
    inline bool operator!=(const VkbInputKey &other) const;

    Qt::Key key = Qt::Key_unknown;
    QString text;
    QStringList alt;
    qreal span = 1.0;
    bool autoRepeat = false;
    bool checkable = false;
    bool checked = false;
};

bool VkbInputKey::operator==(const VkbInputKey &other) const
{
    return key == other.key && alt == other.alt && qFuzzyCompare(span, other.span)
            && autoRepeat == other.autoRepeat && checkable == other.checkable && checked == other.checked;
}

bool VkbInputKey::operator!=(const VkbInputKey &other) const
{
    return !(*this == other);
}

inline uint qHash(const VkbInputKey &key, uint seed)
{
    return qHash(key.key, seed) ^ qHash(key.text, seed);
}

#endif // VKBINPUTKEY_H
