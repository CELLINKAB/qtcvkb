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
#include "vkbinputcontext_p.h"
#include "vkbinputintegration_p.h"
#include "vkbinputlayout.h"

#include <QtCore/qrect.h>
#include <QtCore/qlocale.h>
#include <QtGui/qevent.h>

VkbInputContext::VkbInputContext(const QStringList &params)
    : d_ptr(new VkbInputContextPrivate)
{
    Q_D(VkbInputContext);
    d->q_ptr = this;
    VkbInputIntegrationPrivate::load(params);
    d->inputPanel.setLayout(d->inputEngine.layout());

    connect(&d->inputEngine, &VkbInputEngine::keyPressAndHold, &d->inputPanel, &VkbInputPanelProxy::popup);
    connect(&d->inputEngine, &VkbInputEngine::layoutChanged, &d->inputPanel, &VkbInputPanelProxy::setLayout);

    connect(&d->inputPanel, &VkbInputPanelProxy::keyPressed, &d->inputEngine, &VkbInputEngine::handleKeyPress);
    connect(&d->inputPanel, &VkbInputPanelProxy::keyReleased, &d->inputEngine, &VkbInputEngine::handleKeyRelease);
    connect(&d->inputPanel, &VkbInputPanelProxy::keyCanceled, &d->inputEngine, &VkbInputEngine::handleKeyCancel);
    connect(&d->inputPanel, &VkbInputPanelProxy::keyPressAndHold, &d->inputEngine, &VkbInputEngine::handleKeyPressAndHold);

    connect(&d->inputPanel, &VkbInputPanelProxy::visibleChanged, this, &QPlatformInputContext::emitInputPanelVisibleChanged);
    connect(&d->inputPanel, &VkbInputPanelProxy::animatingChanged, this, &QPlatformInputContext::emitAnimatingChanged);
    connect(&d->inputPanel, &VkbInputPanelProxy::rectChanged, this, &QPlatformInputContext::emitKeyboardRectChanged);
    connect(&d->inputPanel, &VkbInputPanelProxy::localeChanged, this, &QPlatformInputContext::emitLocaleChanged);
    connect(&d->inputPanel, &VkbInputPanelProxy::inputDirectionChanged, this, &QPlatformInputContext::emitInputDirectionChanged);
}

VkbInputContext::~VkbInputContext()
{
}

bool VkbInputContext::isValid() const
{
    return VkbInputIntegration::isValid();
}

void VkbInputContext::reset()
{
    Q_D(VkbInputContext);
    d->inputEngine.reset();
}

void VkbInputContext::commit()
{
    Q_D(VkbInputContext);
    d->inputEngine.commit();
}

void VkbInputContext::update(Qt::InputMethodQueries queries)
{
    Q_D(VkbInputContext);
    d->inputEngine.update(queries);
}

bool VkbInputContext::filterEvent(const QEvent *event)
{
    Q_D(VkbInputContext);
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        d->inputEngine.setKeyboardModifiers(static_cast<const QKeyEvent *>(event)->modifiers());
        d->inputSelection.hide();
    }

    return QPlatformInputContext::filterEvent(event);
}

QRectF VkbInputContext::keyboardRect() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel.rect();
}

bool VkbInputContext::isAnimating() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel.isAnimating();
}

void VkbInputContext::showInputPanel()
{
    Q_D(VkbInputContext);
    d->inputPanel.show();
}

void VkbInputContext::hideInputPanel()
{
    Q_D(VkbInputContext);
    d->inputPanel.hide();
}

bool VkbInputContext::isInputPanelVisible() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel.isVisible();
}

QLocale VkbInputContext::locale() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel.locale();
}

Qt::LayoutDirection VkbInputContext::inputDirection() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel.inputDirection();
}

void VkbInputContext::setFocusObject(QObject *focusObject)
{
    Q_D(VkbInputContext);
    if (!inputMethodAccepted())
        d->inputPanel.hide();

    d->inputEngine.setFocusObject(focusObject);
    d->inputSelection.setFocusObject(focusObject);
}
