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
#include "vkbinputintegration.h"
#include "vkbinputintegrationplugin.h"

#include <QtCore/qrect.h>
#include <QtCore/qlocale.h>
#include <QtCore/private/qfactoryloader_p.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qevent.h>
#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformintegration.h>

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, integrationLoader, (VkbInputIntegrationPlugin_iid, QLatin1String("/cvkbintegrations"), Qt::CaseInsensitive))

static QString resolveIntegration(const QString &name)
{
    if (!name.isEmpty())
        return name;

    const QCoreApplication *app = QCoreApplication::instance();
    if (app && app->inherits("QApplication"))
        return QStringLiteral("widgets");

    return QStringLiteral("quick");
}

static VkbInputIntegration *loadIntegration(const QStringList &params)
{
    const QString name = resolveIntegration(params.value(0));
    return qLoadPlugin<VkbInputIntegration, VkbInputIntegrationPlugin>(integrationLoader(), name, params.mid(1));
}

VkbInputContext::VkbInputContext(const QStringList &params)
    : d_ptr(new VkbInputContextPrivate)
{
    Q_D(VkbInputContext);
    d->q_ptr = this;
    d->inputIntegration = loadIntegration(params);

    connect(&d->inputEngine, &VkbInputEngine::inputModeChanged, [=]() { d->loadInputLayout(); });
    connect(&d->inputEngine, SIGNAL(keyPressAndHold(VkbInputKey)), this, SLOT(_q_showInputPopup(VkbInputKey)));
}

VkbInputContext::~VkbInputContext()
{
}

VkbInputContext *VkbInputContext::instance()
{
    return qobject_cast<VkbInputContext *>(QGuiApplicationPrivate::platformIntegration()->inputContext());
}

bool VkbInputContext::isValid() const
{
    Q_D(const VkbInputContext);
    return d->inputIntegration;
}

bool VkbInputContext::hasCapability(Capability capability) const
{
    return QPlatformInputContext::hasCapability(capability);
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
    QPlatformInputContext::update(queries);
}

void VkbInputContext::invokeAction(QInputMethod::Action action, int cursorPosition)
{
    QPlatformInputContext::invokeAction(action, cursorPosition);
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
    return d->inputPanel()->rect();
}

bool VkbInputContext::isAnimating() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel()->isAnimating();
}

void VkbInputContext::showInputPanel()
{
    Q_D(VkbInputContext);
    d->createInputPanel()->setVisible(true);
}

void VkbInputContext::hideInputPanel()
{
    Q_D(VkbInputContext);
    d->inputPanel()->setVisible(false);
}

bool VkbInputContext::isInputPanelVisible() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel()->isVisible();
}

QLocale VkbInputContext::locale() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel()->locale();
}

Qt::LayoutDirection VkbInputContext::inputDirection() const
{
    Q_D(const VkbInputContext);
    return d->inputPanel()->inputDirection();
}

void VkbInputContext::setFocusObject(QObject *focusObject)
{
    Q_D(VkbInputContext);
    bool enabled = inputMethodAccepted();
    Qt::InputMethodHints inputMethodHints = Qt::ImhNone;
    if (focusObject) {
        QInputMethodQueryEvent event(Qt::ImHints);
        QCoreApplication::sendEvent(focusObject, &event);
        inputMethodHints = static_cast<Qt::InputMethodHints>(event.value(Qt::ImHints).toUInt());
    }

    if (!enabled)
        hideInputPanel();

    d->inputEngine.setInputMethodHints(inputMethodHints);
    d->inputSelection.setEnabled(enabled && !inputMethodHints.testFlag(Qt::ImhNoTextHandles));
    d->inputSelection.setFocusObject(focusObject);
}

#include "moc_vkbinputcontext.cpp"
