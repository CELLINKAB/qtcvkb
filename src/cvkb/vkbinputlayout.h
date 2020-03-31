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

#ifndef VKBINPUTLAYOUT_H
#define VKBINPUTLAYOUT_H

#include <QtCVkb/vkbglobal.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstringlist.h>

class VkbInputLayoutPrivate;

class Q_CVKB_EXPORT VkbInputLayoutItem
{
public:
    QString key;
    QStringList alt;
    qreal span = 1.0;
    bool autoRepeat = false;
    bool checkable = false;
    bool checked = false;
};

class Q_CVKB_EXPORT VkbInputLayout
{
public:
    VkbInputLayout();
    VkbInputLayout(const VkbInputLayout &other);
    ~VkbInputLayout();

    VkbInputLayout& operator=(const VkbInputLayout &other);
    bool operator==(const VkbInputLayout &other) const;
    bool operator!=(const VkbInputLayout &other) const;

    int rowCount() const;
    int columnCount() const;
    VkbInputLayoutItem itemAt(int row, int column) const;

    bool load(const QString &filePath);

private:
    Q_DECLARE_PRIVATE(VkbInputLayout)
    QExplicitlySharedDataPointer<VkbInputLayoutPrivate> d_ptr;
};

#endif // VKBINPUTLAYOUT_H