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

#include "vkbquicklayout.h"

#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>
#include <QtCVkb/vkbinputgrid.h>

VkbQuickLayout::VkbQuickLayout(QQuickItem *parent) : QQuickItem(parent)
{
}

qreal VkbQuickLayout::spacing() const
{
    return m_spacing;
}

void VkbQuickLayout::setSpacing(qreal spacing)
{
    if (qFuzzyCompare(m_spacing, spacing))
        return;

    m_spacing = spacing;
    polish();
    updateImplicitSize();
    emit spacingChanged();
}

VkbInputLayout VkbQuickLayout::layout() const
{
    return m_layout;
}

void VkbQuickLayout::setLayout(const VkbInputLayout &layout)
{
    if (m_layout == layout)
        return;

    m_layout = layout;
    polish();
    updateImplicitSize();
    emit layoutChanged();
}

QHash<VkbInputKey, QQuickAbstractButton *> VkbQuickLayout::buttons() const
{
    return m_buttons;
}

void VkbQuickLayout::setButtons(const QHash<VkbInputKey, QQuickAbstractButton *> &buttons)
{
    if (m_buttons == buttons)
        return;

    m_buttons = buttons;
    polish();
    updateImplicitSize();
    emit buttonsChanged();
}

void VkbQuickLayout::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    if (newGeometry.size() != oldGeometry.size())
        polish();
}

void VkbQuickLayout::updatePolish()
{
    QQuickItem::updatePolish();
    relayout();
}

void VkbQuickLayout::relayout()
{
    VkbInputGrid grid(m_layout);
    grid.setSize(width(), height());
    grid.setSpacing(m_spacing);

    for (int r = 0; r < grid.rowCount(); ++r) {
        for (int c = 0; c < grid.columnCount(); ++c) {
            const VkbInputKey key = grid.keyAt(r, c);
            QQuickAbstractButton *button = m_buttons.value(key);
            if (!button)
                continue;

            const QRectF geometry = grid.geometryAt(r, c);
            button->setPosition(geometry.topLeft());
            button->setSize(geometry.size());
        }
    }
}

void VkbQuickLayout::updateImplicitSize()
{
    qreal width = 0;
    qreal height = 0;

    const int rows = m_layout.rowCount();
    for (int r = 0; r < rows; ++r) {
        qreal rowWidth = 0;
        qreal rowHeight = 0;
        const QVector<VkbInputKey> row = m_layout.rowAt(r);
        for (int c = 0; c < row.count(); ++c) {
            const VkbInputKey key = row.at(c);
            QQuickAbstractButton *button = m_buttons.value(key);
            if (!button)
                continue;

            rowWidth += button->implicitWidth() + m_spacing;
            rowHeight = std::max(rowHeight, button->implicitHeight());
        }

        width = std::max(width, rowWidth - m_spacing);
        height += rowHeight + m_spacing;
    }

    setImplicitSize(width, height - m_spacing);
}
