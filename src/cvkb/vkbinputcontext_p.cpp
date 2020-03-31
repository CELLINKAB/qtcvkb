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

bool VkbInputContextPrivate::createInputPanel(QObject *parent)
{
    if (inputPanel.isNull() && inputPanelFactory)
        inputPanel = inputPanelFactory(parent);
    return !inputPanel.isNull();
}

bool VkbInputContextPrivate::isInputPanelVisible() const
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip)
        return false;

    return ip->isVisible();
}

void VkbInputContextPrivate::showInputPanel()
{
    if (!createInputPanel(QGuiApplication::focusWindow()))
        return;

    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip || ip->isVisible())
        return;

    ip->setVisible(true);
}

void VkbInputContextPrivate::hideInputPanel()
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip || !ip->isVisible())
        return;

    ip->setVisible(false);
}

bool VkbInputContextPrivate::isInputPanelAnimating() const
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip)
        return false;

    return ip->isAnimating();
}

QRectF VkbInputContextPrivate::inputPanelRect() const
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip)
        return QRectF();

    return ip->rect();
}

QLocale VkbInputContextPrivate::inputPanelLocale() const
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip)
        return QLocale::system();

    return ip->locale();
}

Qt::LayoutDirection VkbInputContextPrivate::inputPanelDirection() const
{
    VkbInputPanelInterface *ip = qobject_cast<VkbInputPanelInterface *>(inputPanel);
    if (!ip)
        return Qt::LeftToRight;

    return ip->inputDirection();
}

void VkbInputContextPrivate::_q_emitInputPanelVisibleChanged()
{
    Q_Q(VkbInputContext);
    q->emitInputPanelVisibleChanged();
}

void VkbInputContextPrivate::_q_emitAnimatingChanged()
{
    Q_Q(VkbInputContext);
    q->emitInputPanelVisibleChanged();
}

void VkbInputContextPrivate::_q_emitKeyboardRectChanged()
{
    Q_Q(VkbInputContext);
    q->emitInputPanelVisibleChanged();
}

void VkbInputContextPrivate::_q_emitLocaleChanged()
{
    Q_Q(VkbInputContext);
    q->emitInputPanelVisibleChanged();
}

void VkbInputContextPrivate::_q_emitInputDirectionChanged()
{
    Q_Q(VkbInputContext);
    q->emitInputDirectionChanged(inputPanelDirection());
}
