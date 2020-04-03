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

#ifndef VKBWIDGETSPOPUP_H
#define VKBWIDGETSPOPUP_H

#include <QtCore/qhash.h>
#include <QtWidgets/qwidget.h>
#include <QtCVkb/vkbinputkey.h>
#include <QtCVkb/vkbinputpopup.h>

QT_FORWARD_DECLARE_CLASS(QAbstractButton)

class VkbWidgetsPopup : public QWidget, public VkbInputPopup
{
    Q_OBJECT
    Q_INTERFACES(VkbInputPopup)

public:
    explicit VkbWidgetsPopup(QObject *parent = nullptr);

    void show() override;
    void hide() override;

    void setAlt(const QStringList &alt) override;

signals:
    void keyPressed(const VkbInputKey &key) override;
    void keyReleased(const VkbInputKey &key) override;
    void keyCanceled(const VkbInputKey &key) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void handlePress(QAbstractButton *button);
    void handleRelease(QAbstractButton *button);
    void handleCancel(QAbstractButton *button);

private:
    void updateButtons();
    void updateCurrentButton(QAbstractButton *button);

    QStringList m_alt;
    QAbstractButton *m_currentButton = nullptr;
    QHash<VkbInputKey, QAbstractButton *> m_buttons;
};

#endif // VKBWIDGETSPOPUP_H
