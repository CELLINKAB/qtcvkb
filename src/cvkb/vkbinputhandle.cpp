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

#include "vkbinputhandle_p.h"
#include "vkbinputintegration.h"

#include <QtGui/qguiapplication.h>
#include <QtGui/qwindow.h>

class VkbInputNullHandle : public VkbInputHandle
{
public:
    void show() override { }
    void hide() override { }
    QPointF pos() const override { return QPointF(); }
    QSizeF size() const override { return QSizeF(); }
    void move(const QPointF &) override { }
    void pressed(const QPointF &) override { }
    void released(const QPointF &) override { }
    void moved(const QPointF &) override { }
    void canceled() override { }
};

VkbInputHandleProxy::VkbInputHandleProxy(QObject *parent) : QObject(parent)
{
}

bool VkbInputHandleProxy::isNull() const
{
    return m_instance.isNull();
}

void VkbInputHandleProxy::show()
{
    create()->show();
}

void VkbInputHandleProxy::hide()
{
    instance()->hide();
}

QPointF VkbInputHandleProxy::pos() const
{
    return instance()->pos();
}

QSizeF VkbInputHandleProxy::size() const
{
    return instance()->size();
}

void VkbInputHandleProxy::move(const QPointF &pos)
{
    create()->move(pos);
}

VkbInputHandle *VkbInputHandleProxy::create()
{
    if (!m_instance) {
        m_instance = createHandle(QGuiApplication::focusWindow());
        if (m_instance) {
            QObject::connect(m_instance, SIGNAL(pressed(QPointF)), this, SIGNAL(pressed(QPointF)));
            QObject::connect(m_instance, SIGNAL(released(QPointF)), this, SIGNAL(released(QPointF)));
            QObject::connect(m_instance, SIGNAL(moved(QPointF)), this, SIGNAL(moved(QPointF)));
            QObject::connect(m_instance, SIGNAL(canceled()), this, SIGNAL(canceled()));
        }
    }
    return instance();
}

VkbInputHandle *VkbInputHandleProxy::instance() const
{
    VkbInputHandle *inputHandle = qobject_cast<VkbInputHandle *>(m_instance);
    if (!inputHandle) {
        static VkbInputNullHandle nullHandle;
        return &nullHandle;
    }
    return inputHandle;
}

VkbInputCursorProxy::VkbInputCursorProxy(QObject *parent) : VkbInputHandleProxy(parent)
{
}

QObject *VkbInputCursorProxy::createHandle(QObject *parent)
{
    return VkbInputIntegration::instance()->createInputCursor(parent);
}

VkbInputAnchorProxy::VkbInputAnchorProxy(QObject *parent) : VkbInputHandleProxy(parent)
{
}

QObject *VkbInputAnchorProxy::createHandle(QObject *parent)
{
    return VkbInputIntegration::instance()->createInputAnchor(parent);
}
