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

#ifndef VKBINPUTPOPUP_H
#define VKBINPUTPOPUP_H

#include <QtQuickTemplates2/private/qquickpopup_p.h>
#include <QtQml/qqmllist.h>

class VkbInputKey;
class VkbInputModel;
class VkbInputDelegate;
class VkbInputLayoutItem;

QT_FORWARD_DECLARE_CLASS(QQuickAbstractButton)

class VkbInputPopup : public QQuickPopup
{
    Q_OBJECT
    Q_PROPERTY(QStringList alt READ alt WRITE setAlt NOTIFY altChanged)
    Q_PROPERTY(QQmlListProperty<VkbInputDelegate> delegates READ delegates)

public:
    explicit VkbInputPopup(QObject *parent = nullptr);

    QStringList alt() const;
    void setAlt(const QStringList &alt);

    QQmlListProperty<VkbInputDelegate> delegates();

    void setVisible(bool visible) override;

signals:
    void altChanged();
    void keySelected(const VkbInputKey &key);

protected:
    void componentComplete() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseUngrabEvent() override;

private:
    void updateSpacing();
    void updateButtons();
    void updateCurrentButton(QQuickAbstractButton *button);

    QStringList m_alt;
    VkbInputModel *m_model = nullptr;
    VkbInputLayoutItem *m_layoutItem = nullptr;
    QQuickAbstractButton *m_currentButton = nullptr;
};

#endif // VKBINPUTPOPUP_H
