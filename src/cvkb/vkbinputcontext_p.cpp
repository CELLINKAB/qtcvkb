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
#include "vkbinputintegration.h"
#include "vkbinputintegrationplugin.h"
#include "vkbinputlayout.h"
#include "vkbinputnullobject_p.h"

#include <QtCore/private/qfactoryloader_p.h>
#include <QtGui/qguiapplication.h>

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (VkbInputIntegrationPlugin_iid, QLatin1String("/cvkbintegrations"), Qt::CaseInsensitive))

static QString resolveIntegration(const QString &name)
{
    if (!name.isEmpty())
        return name;

    const QCoreApplication *app = QCoreApplication::instance();
    if (app && app->inherits("QApplication"))
        return QStringLiteral("widgets");

    return QStringLiteral("quick");
}

void VkbInputContextPrivate::loadIntegration(const QStringList &params)
{
    const QString name = resolveIntegration(params.value(0));
    inputIntegration = qLoadPlugin<VkbInputIntegration, VkbInputIntegrationPlugin>(loader(), name, params.mid(1));
}

VkbInputPanel *VkbInputContextPrivate::inputPanel() const
{
    VkbInputPanel *inputPanel = qobject_cast<VkbInputPanel *>(inputPanelObject);
    if (!inputPanel) {
        static VkbInputNullPanel nullPanel;
        return &nullPanel;
    }
    return inputPanel;
}

VkbInputPanel *VkbInputContextPrivate::createInputPanel()
{
    Q_Q(VkbInputContext);
    if (!inputPanelObject && inputIntegration) {
        inputPanelObject = inputIntegration->createInputPanel(QGuiApplication::focusWindow());
        if (inputPanelObject) {
            QObject::connect(inputPanelObject, SIGNAL(keyPressed(VkbInputKey)), q, SLOT(_q_handleKeyPress(VkbInputKey)));
            QObject::connect(inputPanelObject, SIGNAL(keyReleased(VkbInputKey)), q, SLOT(_q_handleKeyRelease(VkbInputKey)));
            QObject::connect(inputPanelObject, SIGNAL(keyCanceled(VkbInputKey)), q, SLOT(_q_handleKeyCancel(VkbInputKey)));
            QObject::connect(inputPanelObject, SIGNAL(keyPressAndHold(VkbInputKey)), q, SLOT(_q_handleKeyPressAndHold(VkbInputKey)));
            QObject::connect(inputPanelObject, SIGNAL(visibleChanged()), q, SLOT(_q_emitInputPanelVisibleChanged()));
            QObject::connect(inputPanelObject, SIGNAL(animatingChanged()), q, SLOT(_q_emitAnimatingChanged()));
            QObject::connect(inputPanelObject, SIGNAL(rectChanged()), q, SLOT(_q_emitKeyboardRectChanged()));
            QObject::connect(inputPanelObject, SIGNAL(localeChanged()), q, SLOT(_q_emitLocaleChanged()));
            QObject::connect(inputPanelObject, SIGNAL(inputDirectionChanged()), q, SLOT(_q_emitInputDirectionChanged()));
            loadInputLayout();
        }
    }
    return inputPanel();
}

VkbInputPopup *VkbInputContextPrivate::createInputPopup(const VkbInputKey &key)
{
    Q_Q(VkbInputContext);
    if (!inputIntegration)
        return nullptr;

    QObject *button = inputPanel()->button(key);
    if (!button)
        return nullptr;

    QObject *inputPopup = inputIntegration->createInputPopup(button);
    if (inputPopup) {
        QObject::connect(inputPopup, SIGNAL(keyPressed(VkbInputKey)), q, SLOT(_q_handleKeyPress(VkbInputKey)));
        QObject::connect(inputPopup, SIGNAL(keyReleased(VkbInputKey)), q, SLOT(_q_handleKeyRelease(VkbInputKey)));
        QObject::connect(inputPopup, SIGNAL(keyCanceled(VkbInputKey)), q, SLOT(_q_handleKeyCancel(VkbInputKey)));
    }
    return qobject_cast<VkbInputPopup *>(inputPopup);
}

static QString resolveInputLayout(VkbInputEngine::InputMode inputMode)
{
    static const QString LayoutPath = qEnvironmentVariable("CVKB_LAYOUT_PATH", QStringLiteral(":/cvkb/layouts"));

    const QMetaEnum metaEnum = QMetaEnum::fromType<VkbInputEngine::InputMode>();
    const QString layoutName = QString::fromLatin1(metaEnum.key(inputMode)).toLower();

    return QDir(LayoutPath).filePath(layoutName + QStringLiteral(".json"));
}

bool VkbInputContextPrivate::loadInputLayout()
{
    VkbInputLayout layout;
    if (!layout.load(resolveInputLayout(inputEngine.inputMode())))
        return false;

    createInputPanel()->setLayout(layout);
    return true;
}

void VkbInputContextPrivate::_q_handleKeyPress(const VkbInputKey &key)
{
    inputEngine.handleKeyPress(key);
}

void VkbInputContextPrivate::_q_handleKeyRelease(const VkbInputKey &key)
{
    inputEngine.handleKeyRelease(key);
}

void VkbInputContextPrivate::_q_handleKeyCancel(const VkbInputKey &key)
{
    inputEngine.handleKeyCancel(key);
}

void VkbInputContextPrivate::_q_handleKeyPressAndHold(const VkbInputKey &key)
{
    inputEngine.handleKeyPressAndHold(key);

    if (key.alt.isEmpty())
        return;

    VkbInputPopup *popup = createInputPopup(key);
    if (!popup)
        return;

    popup->setAlt(key.alt);
    popup->show();
}

void VkbInputContextPrivate::_q_emitInputPanelVisibleChanged()
{
    Q_Q(VkbInputContext);
    q->emitInputPanelVisibleChanged();
}

void VkbInputContextPrivate::_q_emitAnimatingChanged()
{
    Q_Q(VkbInputContext);
    q->emitAnimatingChanged();
}

void VkbInputContextPrivate::_q_emitKeyboardRectChanged()
{
    Q_Q(VkbInputContext);
    q->emitKeyboardRectChanged();
}

void VkbInputContextPrivate::_q_emitLocaleChanged()
{
    Q_Q(VkbInputContext);
    q->emitLocaleChanged();
}

void VkbInputContextPrivate::_q_emitInputDirectionChanged()
{
    Q_Q(VkbInputContext);
    q->emitInputDirectionChanged(inputPanel()->inputDirection());
}
