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

#ifndef VKBINPUTPANEL_H
#define VKBINPUTPANEL_H

#include <QtQuickTemplates2/private/qquickpopup_p.h>
#include <QtQml/qqmllist.h>
#include <QtCVkb/vkbinputpanelinterface.h>
#include <QtCVkb/vkbinputlayout.h>

class VkbInputPopup;
class VkbInputDelegate;
class VkbInputLayoutItem;

QT_FORWARD_DECLARE_CLASS(QQuickAbstractButton)

class VkbInputPanel : public QQuickPopup, public VkbInputPanelInterface
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<VkbInputDelegate> delegates READ delegates)
    Q_INTERFACES(VkbInputPanelInterface)

public:
    explicit VkbInputPanel(QObject *parent = nullptr);

    bool isVisible() const override;
    void setVisible(bool visible) override;

    bool isAnimating() const override;
    QRectF rect() const override;
    QLocale locale() const override;
    Qt::LayoutDirection inputDirection() const override;

    VkbInputLayout layout() const override;
    void setLayout(const VkbInputLayout &layout) override;

    QQmlListProperty<VkbInputDelegate> delegates();

signals:
    void visibleChanged() override;
    void animatingChanged() override;
    void rectChanged() override;
    void localeChanged() override;
    void inputDirectionChanged() override;
    void layoutChanged() override;

    void keyClicked(const VkbInputKey &key) override;
    void keyPressAndHold(const VkbInputKey &key);

protected slots:
    void handleKeyClick();
    void handleKeyPressAndHold();

protected:
    void componentComplete() override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void spacingChange(qreal newSpacing, qreal oldSpacing) override;

private:
    void updateRect(const QRectF &rect);
    void updateButtons();

    VkbInputDelegate *findDelegate(Qt::Key key) const;
    QQuickAbstractButton *createButton(const VkbInputKey &key, QQuickItem *parent) const;
    VkbInputPopup *createPopup(const VkbInputKey &key, QQuickAbstractButton *button) const;

    static void delegates_append(QQmlListProperty<VkbInputDelegate> *property, VkbInputDelegate *delegate);
    static int delegates_count(QQmlListProperty<VkbInputDelegate> *property);
    static VkbInputDelegate *delegates_at(QQmlListProperty<VkbInputDelegate> *property, int index);
    static void delegates_clear(QQmlListProperty<VkbInputDelegate> *property);

    QRectF m_rect;
    VkbInputLayout m_layout;
    QList<VkbInputDelegate *> m_delegates;
    VkbInputLayoutItem *m_layoutItem = nullptr;
};

#endif // VKBINPUTPANEL_H
