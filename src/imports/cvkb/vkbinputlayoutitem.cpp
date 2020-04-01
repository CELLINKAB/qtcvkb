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

#include "vkbinputlayoutitem.h"

#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbInputLayoutItem::VkbInputLayoutItem(QQuickItem *parent) : QQuickItem(parent)
{
}

qreal VkbInputLayoutItem::spacing() const
{
    return m_spacing;
}

void VkbInputLayoutItem::setSpacing(qreal spacing)
{
    if (qFuzzyCompare(m_spacing, spacing))
        return;

    m_spacing = spacing;
    polish();
    emit spacingChanged();
}

VkbInputLayout VkbInputLayoutItem::layout() const
{
    return m_layout;
}

void VkbInputLayoutItem::setLayout(const VkbInputLayout &layout)
{
    if (m_layout == layout)
        return;

    m_layout = layout;
    polish();
    emit layoutChanged();
}

QHash<VkbInputKey, QQuickAbstractButton *> VkbInputLayoutItem::buttons() const
{
    return m_buttons;
}

void VkbInputLayoutItem::setButtons(const QHash<VkbInputKey, QQuickAbstractButton *> &buttons)
{
    if (m_buttons == buttons)
        return;

    m_buttons = buttons;
    polish();
    emit buttonsChanged();
}

void VkbInputLayoutItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if (newGeometry.size() != oldGeometry.size())
        polish();
}

void VkbInputLayoutItem::updatePolish()
{
    QQuickItem::updatePolish();
    relayout();
}

void VkbInputLayoutItem::relayout()
{
    const qreal layoutWidth = width();
    const qreal layoutHeight = height();
    const int rowCount = m_layout.rowCount();
    const int maxColumnCount = m_layout.columnCount();
    const qreal itemHeight = (layoutHeight - (rowCount - 1) * m_spacing) / rowCount;
    const qreal itemWidth = std::min((layoutWidth - (maxColumnCount - 1) * m_spacing) / maxColumnCount, itemHeight * 1.5);

    qreal y = 0;
    for (int r = 0; r < rowCount; ++r) {
        qreal x = 0;
        QList<QQuickAbstractButton *> buttons;

        const QVector<VkbInputKey> row = m_layout.rowAt(r);
        const int columnCount = row.count();
        for (int c = 0; c < columnCount; ++c) {
            const VkbInputKey &key = row.at(c);
            QQuickAbstractButton *button = m_buttons.value(key);
            if (!button)
                continue;

            const qreal effectiveItemWidth = itemWidth * key.span + m_spacing * (key.span - 1.0);
            button->setPosition(QPointF(x, y));
            button->setSize(QSize(effectiveItemWidth, itemHeight));
            x += effectiveItemWidth + m_spacing;
            buttons += button;
        }

        const qreal rw = x - m_spacing;
        const qreal dx = (layoutWidth - rw) / 2.0;
        for (QQuickItem *item : qAsConst(buttons))
            item->setX(item->x() + dx);

        y += itemHeight + m_spacing;
    }
}
