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

#include <QtCore/private/qobject_p.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qstylehints.h>

class VkbInputStylePrivate : public QObjectPrivate
{
public:
    int pressAndHoldInterval = 0;
    static VkbInputStyle *instance;
};

VkbInputStyle *VkbInputStylePrivate::instance = nullptr;

#define vkbStyle VkbInputStylePrivate::instance

VkbInputStyle::VkbInputStyle(QObject *parent)
    : QObject(*(new VkbInputStylePrivate), parent)
{
    Q_D(VkbInputStyle);
    d->pressAndHoldInterval = QGuiApplication::styleHints()->mousePressAndHoldInterval();

    Q_ASSERT(!vkbStyle);
    vkbStyle = this;
}

VkbInputStyle::~VkbInputStyle()
{
    vkbStyle = nullptr;
}

VkbInputStyle *VkbInputStyle::instance()
{
    if (!vkbStyle)
        vkbStyle = new VkbInputStyle(QCoreApplication::instance());
    return vkbStyle;
}

int VkbInputStyle::pressAndHoldInterval()
{
    Q_D(VkbInputStyle);
    return d->pressAndHoldInterval;
}

void VkbInputStyle::setPressAndHoldInterval(int pressAndHoldInterval)
{
    Q_D(VkbInputStyle);
    if (d->pressAndHoldInterval == pressAndHoldInterval)
        return;

    d->pressAndHoldInterval = pressAndHoldInterval;
    emit pressAndHoldIntervalChanged(pressAndHoldInterval);
}
