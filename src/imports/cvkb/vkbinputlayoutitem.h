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

#ifndef VKBINPUTLAYOUTITEM_H
#define VKBINPUTLAYOUTITEM_H

#include <QtCore/qhash.h>
#include <QtQml/qqmllist.h>
#include <QtQuick/qquickitem.h>
#include <QtCVkb/vkbinputlayout.h>

class VkbInputPopup;
class VkbInputLayoutAttached;
class VkbInputLayoutDelegate;

QT_FORWARD_DECLARE_CLASS(QQuickAbstractButton)

class VkbInputLayoutItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(QQmlListProperty<VkbInputLayoutDelegate> delegates READ delegates)
    Q_CLASSINFO("DefaultProperty", "delegates")

public:
    explicit VkbInputLayoutItem(QQuickItem *parent = nullptr);

    qreal spacing() const;
    void setSpacing(qreal spacing);

    VkbInputLayout layout() const;
    void setLayout(const VkbInputLayout &layout);

    QQmlListProperty<VkbInputLayoutDelegate> delegates();

    static VkbInputLayoutAttached *qmlAttachedProperties(QObject *object);

signals:
    void spacingChanged();
    void layoutChanged();
    void keyClicked(const VkbInputKey &key);
    void keyPressAndHold(const VkbInputKey &key);

protected slots:
    void handleKeyClick();
    void handleKeyPressAndHold();

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void updatePolish() override;

private:
    VkbInputLayoutDelegate *findDelegate(Qt::Key key) const;
    QQuickAbstractButton *createButton(const VkbInputKey &key, QQuickItem *parent) const;
    VkbInputPopup *createPopup(const VkbInputKey &key, QQuickAbstractButton *button) const;

    static void delegates_append(QQmlListProperty<VkbInputLayoutDelegate> *property, VkbInputLayoutDelegate *delegate);
    static int delegates_count(QQmlListProperty<VkbInputLayoutDelegate> *property);
    static VkbInputLayoutDelegate *delegates_at(QQmlListProperty<VkbInputLayoutDelegate> *property, int index);
    static void delegates_clear(QQmlListProperty<VkbInputLayoutDelegate> *property);

    qreal m_spacing = 0;
    VkbInputLayout m_layout;
    QList<VkbInputLayoutDelegate *> m_delegates;
    QHash<VkbInputKey, QQuickAbstractButton *> m_buttons;
};

#endif // VKBINPUTLAYOUTITEM_H
