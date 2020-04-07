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

#ifndef VKBINPUTPANEL_P_H
#define VKBINPUTPANEL_P_H

#include <QtCVkb/vkbinputpanel.h>
#include <QtCVkb/vkbinputlayout.h>
#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>

class VkbInputKey;

class VkbInputPanelProxy : public QObject, public VkbInputPanel
{
    Q_OBJECT
    Q_INTERFACES(VkbInputPanel)

public:
    explicit VkbInputPanelProxy(QObject *parent = nullptr);

    bool isVisible() const override;
    bool isAnimating() const override;
    QRectF rect() const override;
    QLocale locale() const override;
    Qt::LayoutDirection inputDirection() const override;

    QObject *button(const VkbInputKey &key) const override;

public slots:
    void show() override;
    void hide() override;
    void popup(const VkbInputKey &key);
    void setLayout(const VkbInputLayout &layout) override;

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

private:
    VkbInputPanel *create();
    VkbInputPanel *instance() const;

    VkbInputLayout m_layout;
    QPointer<QObject> m_instance;
};

#endif // VKBINPUTPANEL_P_H
