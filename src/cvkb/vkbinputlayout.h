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

#include <QtCVkb/vkbinputglobal.h>
#include <QtCVkb/vkbinputkey.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstringlist.h>

class VkbInputLayoutPrivate;

class Q_CVKB_EXPORT VkbInputLayout
{
public:
    VkbInputLayout();
    VkbInputLayout(const QVector<VkbInputKey> &row);
    VkbInputLayout(const VkbInputLayout &other);
    ~VkbInputLayout();

    VkbInputLayout& operator=(const VkbInputLayout &other);
    bool operator==(const VkbInputLayout &other) const;
    bool operator!=(const VkbInputLayout &other) const;

    bool isEmpty() const;
    int rowCount() const;
    int columnCount() const;

    QVector<VkbInputKey> rowAt(int row) const;
    void addRow(const QVector<VkbInputKey> &row);
    void insertRow(int index, const QVector<VkbInputKey> &row);

    VkbInputKey keyAt(int row, int column) const;
    void setKey(int row, int column, const VkbInputKey &key);

    VkbInputKey findKey(Qt::Key key, const QString &text) const;

    bool load(const QString &filePath);

private:
    Q_DECLARE_PRIVATE(VkbInputLayout)
    QExplicitlySharedDataPointer<VkbInputLayoutPrivate> d_ptr;
};

#endif // VKBINPUTLAYOUT_H
