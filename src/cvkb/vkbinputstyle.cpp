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

#include "vkbinputstyle.h"
#include "vkbinputstyle_p.h"

#include <QtCore/qcoreapplication.h>

Q_GLOBAL_STATIC(VkbInputStylePrivate, dd)

#define V_D(Class) Class##Private * const d = dd()

VkbInputStyle::VkbInputStyle(QObject *parent) : QObject(parent)
{
    V_D(VkbInputStyle);
    Q_ASSERT(!d->instance);
    d->instance = this;
}

VkbInputStyle::~VkbInputStyle()
{
    V_D(VkbInputStyle);
    d->instance = nullptr;
}

VkbInputStyle *VkbInputStyle::instance()
{
    V_D(VkbInputStyle);
    if (!d->instance)
        d->instance = new VkbInputStyle(QCoreApplication::instance());
    return d->instance;
}

int VkbInputStyle::pressAndHoldInterval()
{
    V_D(VkbInputStyle);
    return d->pressAndHoldInterval;
}

void VkbInputStyle::setPressAndHoldInterval(int pressAndHoldInterval)
{
    V_D(VkbInputStyle);
    if (d->pressAndHoldInterval == pressAndHoldInterval)
        return;

    d->pressAndHoldInterval = pressAndHoldInterval;
    if (d->instance)
        emit d->instance->pressAndHoldIntervalChanged(pressAndHoldInterval);
}
