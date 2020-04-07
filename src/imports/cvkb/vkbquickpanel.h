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

#ifndef VKBQUICKPANEL_H
#define VKBQUICKPANEL_H

#include <QtQuickTemplates2/private/qquickpopup_p.h>
#include <QtQml/qqmllist.h>
#include <QtCVkb/vkbinputpanel.h>

class VkbInputKey;
class VkbInputLayout;
class VkbQuickPopup;
class VkbQuickModel;
class VkbQuickDelegate;
class VkbQuickLayout;

QT_FORWARD_DECLARE_CLASS(QQuickAbstractButton)

class VkbQuickPanel : public QQuickPopup, public VkbInputPanel
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<VkbQuickDelegate> delegates READ delegates)
    Q_INTERFACES(VkbInputPanel)

public:
    explicit VkbQuickPanel(QObject *parent = nullptr);

    bool isVisible() const override;
    void setVisible(bool visible) override;

    bool isAnimating() const override;
    QRectF rect() const override;
    QLocale locale() const override;
    Qt::LayoutDirection inputDirection() const override;

    QObject *button(const VkbInputKey &key) const override;
    void setLayout(const VkbInputLayout &layout) override;

    QQmlListProperty<VkbQuickDelegate> delegates();

signals:
    void visibleChanged() override;
    void animatingChanged() override;
    void rectChanged() override;
    void localeChanged() override;
    void inputDirectionChanged(Qt::LayoutDirection inputDirection) override;

    void keyPressed(const VkbInputKey &key) override;
    void keyReleased(const VkbInputKey &key) override;
    void keyCanceled(const VkbInputKey &key) override;
    void keyPressAndHold(const VkbInputKey &key) override;

protected slots:
    void handleKeyPress();
    void handleKeyRelease();
    void handleKeyCancel();
    void handleKeyPressAndHold();

protected:
    void componentComplete() override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void updateRect(const QRectF &rect);
    void updateSpacing();
    void updateButtons();

    QRectF m_rect;
    VkbQuickModel *m_model = nullptr;
    QList<VkbQuickDelegate *> m_delegates;
    VkbQuickLayout *m_layout = nullptr;
};

#endif // VKBQUICKPANEL_H
