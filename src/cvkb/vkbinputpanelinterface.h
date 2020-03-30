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

#ifndef VKBINPUTPANELINTERFACE_H
#define VKBINPUTPANELINTERFACE_H

#include <QtCVkb/vkbglobal.h>
#include <QtCore/qobject.h>

QT_FORWARD_DECLARE_CLASS(QRectF)
QT_FORWARD_DECLARE_CLASS(QLocaleF)

class Q_CVKB_EXPORT VkbInputPanelInterface
{
public:
    virtual ~VkbInputPanelInterface() = default;

    virtual bool isVisible() const = 0;
    virtual void setVisible(bool visible) = 0;

    virtual bool isAnimating() const = 0;
    virtual QRectF rect() const = 0;
    virtual QLocale locale() const = 0;
    virtual Qt::LayoutDirection inputDirection() const = 0;

signals:
    virtual void visibleChanged(bool visible) = 0;
    virtual void animatingChanged(bool animating) = 0;
    virtual void rectChanged(const QRectF &rect) = 0;
    virtual void localChanged(const QLocale &locale) = 0;
    virtual void inputDirectionChanged(Qt::LayoutDirection inputDirection) = 0;
    virtual void keyClicked(const QString &key) = 0;
};

#define VkbInputPanelInterface_iid "VkbInputPanelInterface/0.1"
Q_DECLARE_INTERFACE(VkbInputPanelInterface, VkbInputPanelInterface_iid)

#endif // VKBINPUTPANELINTERFACE_H
