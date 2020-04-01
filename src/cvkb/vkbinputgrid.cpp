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

#include "vkbinputgrid.h"
#include "vkbinputlayout.h"

#include <QtCore/qrect.h>
#include <QtCore/qsize.h>
#include <QtCore/qvector.h>

#include <cmath>

class VkbInputGridPrivate : public QSharedData
{
public:
    QSizeF itemSize() const;
    void calculate();

    QSizeF size;
    qreal spacing = 0;
    VkbInputLayout layout;
    QVector<QVector<QRectF>> geometries;
};

QSizeF VkbInputGridPrivate::itemSize() const
{
    const int rows = layout.rowCount();
    const int columns = layout.columnCount();
    const qreal h = (size.height() - (rows - 1) * spacing) / rows;
    const qreal w = std::min((size.width() - (columns - 1) * spacing) / columns, h * 1.5);
    return QSizeF(w, h);
}

void VkbInputGridPrivate::calculate()
{
    const QSizeF item = itemSize();

    qreal y = 0;
    const int rowCount = layout.rowCount();
    for (int r = 0; r < rowCount; ++r) {
        qreal x = 0;
        QVector<QRectF> row;
        const QVector<VkbInputKey> keys = layout.rowAt(r);
        for (const VkbInputKey &key : keys) {
            const qreal itemWidth = item.width() * key.span + spacing * (key.span - 1.0);
            row += QRectF(x, y, itemWidth, item.height());
            x += itemWidth + spacing;
        }

        const qreal rowWidth = x - spacing;
        const qreal dx = (size.width() - rowWidth) / 2.0;
        for (int c = 0; c < keys.count(); ++c)
            row[c].translate(dx, 0);
        geometries += row;

        y += item.height() + spacing;
    }
}

VkbInputGrid::VkbInputGrid(const VkbInputLayout &layout) : d_ptr(new VkbInputGridPrivate)
{
    Q_D(VkbInputGrid);
    d->layout = layout;
}

VkbInputGrid::~VkbInputGrid()
{
}

QSizeF VkbInputGrid::size() const
{
    Q_D(const VkbInputGrid);
    return d->size;
}

void VkbInputGrid::setSize(const QSizeF &size)
{
    Q_D(VkbInputGrid);
    d->size = size;
}

void VkbInputGrid::setSize(qreal width, qreal height)
{
    Q_D(VkbInputGrid);
    d->size = QSizeF(width, height);
}

qreal VkbInputGrid::spacing() const
{
    Q_D(const VkbInputGrid);
    return d->spacing;
}

void VkbInputGrid::setSpacing(qreal spacing)
{
    Q_D(VkbInputGrid);
    d->spacing = spacing;
}

int VkbInputGrid::rowCount() const
{
    Q_D(const VkbInputGrid);
    return d->layout.rowCount();
}

int VkbInputGrid::columnCount() const
{
    Q_D(const VkbInputGrid);
    return d->layout.columnCount();
}

VkbInputKey VkbInputGrid::keyAt(int row, int column) const
{
    Q_D(const VkbInputGrid);
    return d->layout.keyAt(row, column);
}

QRectF VkbInputGrid::geometryAt(int row, int column) const
{
    Q_D(const VkbInputGrid);
    if (d->geometries.isEmpty())
        const_cast<VkbInputGridPrivate *>(d)->calculate();
    return d->geometries.value(row).value(column);
}
