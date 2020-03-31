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

#include "vkbinputcontext_p.h"
#include "vkbinputpanelinterface.h"
#include "vkbfactory.h"

static QObject *createInputPanel(QObject *parent)
{
    VkbFactoryFunc factory = VkbFactory::inputPanel();
    if (!factory)
        return nullptr;

    return factory(parent);
}

bool VkbInputContextPrivate::isInputPanelVisible() const
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip)
        return false;

    return ip->isVisible();
}

bool VkbInputContextPrivate::showInputPanel()
{
    if (!inputPanel)
        inputPanel = createInputPanel(QGuiApplication::focusWindow());

    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip || ip->isVisible())
        return false;

    ip->setVisible(true);
    return true;
}

bool VkbInputContextPrivate::hideInputPanel()
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip || !ip->isVisible())
        return false;

    ip->setVisible(false);
    return true;
}
