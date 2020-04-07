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

#include "vkbinputengine.h"
#include "vkbinputengine_p.h"
#include "vkbinputkey.h"

#include <QtGui/qpa/qwindowsysteminterface.h>

VkbInputEngine::VkbInputEngine(QObject *parent)
    : QObject(*(new VkbInputEnginePrivate), parent)
{
}

VkbInputEngine::InputMode VkbInputEngine::inputMode() const
{
    Q_D(const VkbInputEngine);
    return d->inputMode;
}

void VkbInputEngine::setInputMode(InputMode inputMode)
{
    Q_D(VkbInputEngine);
    if (d->inputMode == inputMode)
        return;

    d->inputMode = inputMode;
    d->resolveLayout();
    emit inputModeChanged(inputMode);
}

Qt::InputMethodHints VkbInputEngine::inputMethodHints() const
{
    Q_D(const VkbInputEngine);
    return d->inputMethodHints;
}

void VkbInputEngine::setInputMethodHints(Qt::InputMethodHints inputMethodHints)
{
    Q_D(VkbInputEngine);
    if (d->inputMethodHints == inputMethodHints)
        return;

    d->inputMethodHints = inputMethodHints;
    d->resolveInputMode();
    emit inputMethodHintsChanged(inputMethodHints);
}

Qt::KeyboardModifiers VkbInputEngine::keyboardModifiers() const
{
    Q_D(const VkbInputEngine);
    return d->keyboardModifiers;
}

void VkbInputEngine::setKeyboardModifiers(Qt::KeyboardModifiers keyboardModifiers)
{
    Q_D(VkbInputEngine);
    if (d->keyboardModifiers == keyboardModifiers)
        return;

    d->keyboardModifiers = keyboardModifiers;
    d->resolveInputMode();
    emit keyboardModifiersChanged(keyboardModifiers);
}

VkbInputLayout VkbInputEngine::layout() const
{
    VkbInputEnginePrivate *d = const_cast<VkbInputEnginePrivate *>(d_func());
    if (d->layout.isEmpty())
        d->resolveLayout();
    return d->layout;
}

void VkbInputEngine::setLayout(const VkbInputLayout &layout)
{
    Q_D(VkbInputEngine);
    if (d->layout == layout)
        return;

    d->layout = layout;
    emit layoutChanged(layout);
}

void VkbInputEngine::reset()
{
}

void VkbInputEngine::commit()
{
}

void VkbInputEngine::update(Qt::InputMethodQueries queries)
{
    Q_UNUSED(queries);
}

void VkbInputEngine::setFocusObject(QObject *focusObject)
{
    Q_D(VkbInputEngine);
    if (d->focusObject == focusObject)
        return;

    if (d->focusObject)
        d->focusObject->removeEventFilter(this);

    if (!focusObject) {
        setInputMethodHints(Qt::ImhNone);
        return;
    }

    focusObject->installEventFilter(this);

    QInputMethodQueryEvent event(Qt::ImHints);
    QCoreApplication::sendEvent(focusObject, &event);
    setInputMethodHints(event.value(Qt::ImHints).value<Qt::InputMethodHints>());
}

void VkbInputEngine::sendKeyPress(const VkbInputKey &key)
{
    Q_D(VkbInputEngine);
    typedef std::function<void(VkbInputEnginePrivate *engine, const VkbInputKey &key)> KeyHandler;
    static const QHash<Qt::Key, KeyHandler> handlers = {
        { Qt::Key_Meta, [](VkbInputEnginePrivate *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::MetaModifier); } },
        { Qt::Key_Shift, [](VkbInputEnginePrivate *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::ShiftModifier); } },
        { Qt::Key_Control, [](VkbInputEnginePrivate *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::ControlModifier); } },
        { Qt::Key_Alt, [](VkbInputEnginePrivate *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::AltModifier); } }
    };

    KeyHandler handler = handlers.value(key.key, [=](VkbInputEnginePrivate *engine, const VkbInputKey &key) { engine->sendKeyPress(key); });
    if (handler)
        handler(d, key);
}

void VkbInputEngine::sendKeyRelease(const VkbInputKey &key)
{
    Q_D(VkbInputEngine);
    d->sendKeyRelease(key);
}

bool VkbInputEngine::eventFilter(QObject *object, QEvent *event)
{
    Q_D(VkbInputEngine);
    switch (event->type()) {
    case QEvent::KeyPress: {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        setKeyboardModifiers(keyEvent->modifiers());
        emit keyPressed(d->layout.findKey(static_cast<Qt::Key>(keyEvent->key()), keyEvent->text()));
        break;
    }
    case QEvent::KeyRelease: {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        setKeyboardModifiers(keyEvent->modifiers());
        emit keyReleased(d->layout.findKey(static_cast<Qt::Key>(keyEvent->key()), keyEvent->text()));
        break;
    }
    default:
        break;
    }
    return QObject::eventFilter(object, event);
}

static QString resolveLayoutPath(VkbInputEngine::InputMode inputMode)
{
    static const QString LayoutPath = qEnvironmentVariable("CVKB_LAYOUT_PATH", QStringLiteral(":/cvkb/layouts"));

    const QMetaEnum metaEnum = QMetaEnum::fromType<VkbInputEngine::InputMode>();
    const QString layoutName = QString::fromLatin1(metaEnum.key(inputMode)).toLower();

    return QDir(LayoutPath).filePath(layoutName + QStringLiteral(".json"));
}

void VkbInputEnginePrivate::resolveLayout()
{
    Q_Q(VkbInputEngine);
    VkbInputLayout layout;
    if (!layout.load(resolveLayoutPath(inputMode)))
        return;

    q->setLayout(layout);
}

void VkbInputEnginePrivate::resolveInputMode()
{
    Q_Q(VkbInputEngine);
    if (inputMethodHints & Qt::ImhDigitsOnly)
        q->setInputMode(VkbInputEngine::Digits);
    else if (keyboardModifiers & Qt::MetaModifier)
        q->setInputMode(VkbInputEngine::Symbols);
    else if (keyboardModifiers & Qt::ShiftModifier)
        q->setInputMode(VkbInputEngine::Capitals);
    else
        q->setInputMode(VkbInputEngine::Letters);
}

void VkbInputEnginePrivate::sendKeyPress(const VkbInputKey &key)
{
    if (key.key != Qt::Key_unknown)
        sendKeyEvent(QEvent::KeyPress, key);
}

void VkbInputEnginePrivate::sendKeyRelease(const VkbInputKey &key)
{
    if (key.key != Qt::Key_unknown)
        sendKeyEvent(QEvent::KeyRelease, key);
    else
        sendKeyText(key.text);
}

void VkbInputEnginePrivate::sendKeyText(const QString &text)
{
    QInputMethodEvent event;
    event.setCommitString(text);
    QCoreApplication::sendEvent(QGuiApplication::focusObject(), &event);
}

void VkbInputEnginePrivate::sendKeyEvent(QEvent::Type type, const VkbInputKey &key)
{
    QWindowSystemInterface::handleKeyEvent(QGuiApplication::focusWindow(), type, key.key, keyboardModifiers, key.text);
}

void VkbInputEnginePrivate::toggleKeyboardModifier(Qt::KeyboardModifier modifier)
{
    Q_Q(VkbInputEngine);
    q->setKeyboardModifiers(keyboardModifiers ^ modifier);
}
