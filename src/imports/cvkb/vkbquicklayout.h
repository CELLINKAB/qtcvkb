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
#include <QtQuick/qquickitem.h>
#include <QtCVkb/vkbinputkey.h>
#include <QtCVkb/vkbinputlayout.h>

class VkbQuickLayoutAttached;

QT_FORWARD_DECLARE_CLASS(QQuickAbstractButton)

class VkbQuickLayout : public QQuickItem
{
    Q_OBJECT

public:
    explicit VkbQuickLayout(QQuickItem *parent = nullptr);

    qreal spacing() const;
    void setSpacing(qreal spacing);

    VkbInputLayout inputLayout() const;
    void setInputLayout(const VkbInputLayout &inputLayout);

    QQuickAbstractButton *button(const VkbInputKey &key) const;
    QHash<VkbInputKey, QQuickAbstractButton *> buttons() const;
    void setButtons(const QHash<VkbInputKey, QQuickAbstractButton *> &buttons);

    static VkbQuickLayoutAttached *qmlAttachedProperties(QObject *object);
    static VkbQuickLayoutAttached *qmlAttachedPropertiesObject(QObject *object);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void updatePolish() override;

private:
    void relayout();
    void updateImplicitSize();

    qreal m_spacing = 0;
    VkbInputLayout m_inputLayout;
    QHash<VkbInputKey, QQuickAbstractButton *> m_buttons;
};

class VkbQuickLayoutAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Qt::Key key READ key CONSTANT FINAL)
    Q_PROPERTY(QString text READ text CONSTANT FINAL)
    Q_PROPERTY(QStringList alt READ alt CONSTANT FINAL)
    Q_PROPERTY(bool autoRepeat READ autoRepeat CONSTANT FINAL)
    Q_PROPERTY(bool checkable READ isCheckable CONSTANT FINAL)
    Q_PROPERTY(bool checked READ isChecked CONSTANT FINAL)

public:
    explicit VkbQuickLayoutAttached(QObject *parent = nullptr);

    Qt::Key key() const;
    QString text() const;
    QStringList alt() const;
    bool autoRepeat() const;
    bool isCheckable() const;
    bool isChecked() const;

    VkbInputKey inputKey() const;
    void setInputKey(const VkbInputKey &key);

private:
    VkbInputKey m_key;
};

QML_DECLARE_TYPEINFO(VkbQuickLayout, QML_HAS_ATTACHED_PROPERTIES)

#endif // VKBQUICKLAYOUT_H
