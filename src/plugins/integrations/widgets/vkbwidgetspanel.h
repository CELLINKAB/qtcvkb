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

#ifndef VKBWIDGETSPANEL_H
#define VKBWIDGETSPANEL_H

#include <QtCore/qhash.h>
#include <QtWidgets/qwidget.h>
#include <QtCVkb/vkbinputlayout.h>
#include <QtCVkb/vkbinputpanel.h>

class VkbInputKey;
class VkbWidgetsLayout;

QT_FORWARD_DECLARE_CLASS(QAbstractButton)
QT_FORWARD_DECLARE_CLASS(QGridLayout)

class VkbWidgetsPanel : public QWidget, public VkbInputPanel
{
    Q_OBJECT
    Q_INTERFACES(VkbInputPanel)

public:
    explicit VkbWidgetsPanel(QObject *parent = nullptr);

    bool isVisible() const override;
    void setVisible(bool visible) override;

    bool isAnimating() const override;
    QRectF rect() const override;
    QLocale locale() const override;
    Qt::LayoutDirection inputDirection() const override;

    QObject *button(const VkbInputKey &key) const override;
    void setLayout(const VkbInputLayout &layout) override;

signals:
    void visibleChanged() override;
    void animatingChanged() override;
    void rectChanged() override;
    void localeChanged() override;
    void inputDirectionChanged() override;

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
    void changeEvent(QEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void updateButtons();

    VkbInputLayout m_layout;
    QHash<VkbInputKey, QAbstractButton *> m_buttons;
};

#endif // VKBWIDGETSPANEL_H
