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

#include <QtCVkb/vkbinputglobal.h>
#include <QtCore/qobject.h>

class VkbInputKey;
class VkbInputLayout;

QT_FORWARD_DECLARE_CLASS(QRectF)
QT_FORWARD_DECLARE_CLASS(QLocaleF)

class Q_CVKB_EXPORT VkbInputPanel
{
public:
    virtual ~VkbInputPanel() = default;

    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;

    virtual bool isAnimating() const = 0;
    virtual QRectF rect() const = 0;
    virtual QLocale locale() const = 0;
    virtual Qt::LayoutDirection inputDirection() const = 0;

    virtual QObject *button(const VkbInputKey &key) const = 0;
    virtual void setLayout(const VkbInputLayout &layout) = 0;

signals:
    virtual void visibleChanged() = 0;
    virtual void animatingChanged() = 0;
    virtual void rectChanged() = 0;
    virtual void localeChanged() = 0;
    virtual void inputDirectionChanged() = 0;

    virtual void keyPressed(const VkbInputKey &key) = 0;
    virtual void keyReleased(const VkbInputKey &key) = 0;
    virtual void keyCanceled(const VkbInputKey &key) = 0;
    virtual void keyPressAndHold(const VkbInputKey &key) = 0;
};

#define VkbInputPanel_iid "VkbInputPanel/0.1"
Q_DECLARE_INTERFACE(VkbInputPanel, VkbInputPanel_iid)

#endif // VKBINPUTPANEL_H
