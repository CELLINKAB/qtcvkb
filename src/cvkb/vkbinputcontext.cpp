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

#include "vkbinputcontext.h"

#include <QtCore/qrect.h>
#include <QtCore/qlocale.h>

class VkbInputContextPrivate
{
public:
};

VkbInputContext::VkbInputContext(const QStringList &params)
    : d_ptr(new VkbInputContextPrivate)
{
    Q_UNUSED(params);
}

VkbInputContext::~VkbInputContext()
{
}

bool VkbInputContext::isValid() const
{
    return true;
}

bool VkbInputContext::hasCapability(Capability capability) const
{
    return QPlatformInputContext::hasCapability(capability);
}

void VkbInputContext::reset()
{
    QPlatformInputContext::reset();
}

void VkbInputContext::commit()
{
    QPlatformInputContext::commit();
}

void VkbInputContext::update(Qt::InputMethodQueries queries)
{
    QPlatformInputContext::update(queries);
}

void VkbInputContext::invokeAction(QInputMethod::Action action, int cursorPosition)
{
    QPlatformInputContext::invokeAction(action, cursorPosition);
}

bool VkbInputContext::filterEvent(const QEvent *event)
{
    return QPlatformInputContext::filterEvent(event);
}

QRectF VkbInputContext::keyboardRect() const
{
    return QRectF();
}

bool VkbInputContext::isAnimating() const
{
    return false;
}

void VkbInputContext::showInputPanel()
{
}

void VkbInputContext::hideInputPanel()
{
}

bool VkbInputContext::isInputPanelVisible() const
{
    return false;
}

QLocale VkbInputContext::locale() const
{
    return QPlatformInputContext::locale();
}

Qt::LayoutDirection VkbInputContext::inputDirection() const
{
    return QPlatformInputContext::inputDirection();
}

void VkbInputContext::setFocusObject(QObject *focusObject)
{
    QPlatformInputContext::setFocusObject(focusObject);
}
