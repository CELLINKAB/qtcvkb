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

#ifndef VKBINPUTGRID_H
#define VKBINPUTGRID_H

#include <QtCVkb/vkbglobal.h>
#include <QtCore/qshareddata.h>

class VkbInputKey;
class VkbInputLayout;
class VkbInputGridPrivate;

QT_FORWARD_DECLARE_CLASS(QSizeF)
QT_FORWARD_DECLARE_CLASS(QRectF)

class Q_CVKB_EXPORT VkbInputGrid
{
public:
    VkbInputGrid(const VkbInputLayout &layout);
    ~VkbInputGrid();

    QSizeF size() const;
    void setSize(const QSizeF &size);
    void setSize(qreal width, qreal height);

    qreal spacing() const;
    void setSpacing(qreal spacing);

    int rowCount() const;
    int columnCount() const;

    VkbInputKey keyAt(int row, int column) const;
    QRectF geometryAt(int row, int column) const;

private:
    Q_DECLARE_PRIVATE(VkbInputGrid)
    QExplicitlySharedDataPointer<VkbInputGridPrivate> d_ptr;
};

#endif // VKBINPUTGRID_H
