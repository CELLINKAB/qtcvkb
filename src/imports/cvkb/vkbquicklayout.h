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

#ifndef VKBQUICKLAYOUT_H
#define VKBQUICKLAYOUT_H

#include <QtCore/qhash.h>
#include <QtQml/qqmllist.h>
#include <QtQuick/qquickitem.h>
#include <QtCVkb/vkbinputlayout.h>

QT_FORWARD_DECLARE_CLASS(QQuickAbstractButton)

class VkbQuickLayout : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)

public:
    explicit VkbQuickLayout(QQuickItem *parent = nullptr);

    qreal spacing() const;
    void setSpacing(qreal spacing);

    VkbInputLayout layout() const;
    void setLayout(const VkbInputLayout &layout);

    QHash<VkbInputKey, QQuickAbstractButton *> buttons() const;
    void setButtons(const QHash<VkbInputKey, QQuickAbstractButton *> &buttons);

signals:
    void spacingChanged();
    void layoutChanged();
    void buttonsChanged();

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void updatePolish() override;

private:
    void relayout();
    void updateImplicitSize();

    qreal m_spacing = 0;
    VkbInputLayout m_layout;
    QHash<VkbInputKey, QQuickAbstractButton *> m_buttons;
};

#endif // VKBQUICKLAYOUT_H
