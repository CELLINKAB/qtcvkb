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

#ifndef VKBINPUTHANDLE_H
#define VKBINPUTHANDLE_H

#include <QtCVkb/vkbinputglobal.h>
#include <QtCore/qobject.h>

QT_FORWARD_DECLARE_CLASS(QPointF)
QT_FORWARD_DECLARE_CLASS(QSizeF)

class Q_CVKB_EXPORT VkbInputHandle
{
public:
    virtual ~VkbInputHandle() = default;

    virtual void show() = 0;
    virtual void hide() = 0;

    virtual QPointF pos() const = 0;
    virtual QSizeF size() const = 0;
    virtual void move(const QPointF &pos) = 0;

signals:
    virtual void pressed(const QPointF &pos) = 0;
    virtual void released(const QPointF &pos) = 0;
    virtual void moved(const QPointF &pos) = 0;
    virtual void canceled() = 0;
};

#define VkbInputHandle_iid "VkbInputHandle/0.1"
Q_DECLARE_INTERFACE(VkbInputHandle, VkbInputHandle_iid)

#endif // VKBINPUTHANDLE_H
