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

#ifndef VKBINPUTLAYOUTATTACHED_H
#define VKBINPUTLAYOUTATTACHED_H

#include <QtCore/qobject.h>
#include <QtQml/qqml.h>
#include <QtCVkb/vkbinputlayout.h>

class VkbInputLayoutAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key CONSTANT FINAL)
    Q_PROPERTY(QStringList alt READ alt CONSTANT FINAL)
    Q_PROPERTY(bool autoRepeat READ autoRepeat CONSTANT FINAL)
    Q_PROPERTY(bool checkable READ isCheckable CONSTANT FINAL)
    Q_PROPERTY(bool checked READ isChecked CONSTANT FINAL)

public:
    explicit VkbInputLayoutAttached(QObject *parent = nullptr);

    QString key() const;
    QStringList alt() const;
    bool autoRepeat() const;
    bool isCheckable() const;
    bool isChecked() const;

    void setKey(const VkbInputKey &key);

    static VkbInputLayoutAttached *qmlAttachedProperties(QObject *object);

private:
    VkbInputKey m_key;
};

QML_DECLARE_TYPEINFO(VkbInputLayoutAttached, QML_HAS_ATTACHED_PROPERTIES)

#endif // VKBINPUTLAYOUTATTACHED_H
