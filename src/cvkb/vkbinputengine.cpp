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
#include "vkbinputkey.h"

#include <QtGui/qpa/qwindowsysteminterface.h>

VkbInputEngine::VkbInputEngine(QObject *parent) : QObject(parent)
{
}

VkbInputEngine::InputMode VkbInputEngine::inputMode() const
{
    return m_inputMode;
}

void VkbInputEngine::setInputMode(InputMode inputMode)
{
    if (m_inputMode == inputMode)
        return;

    m_inputMode = inputMode;
    emit inputModeChanged();
}

Qt::InputMethodHints VkbInputEngine::inputMethodHints() const
{
    return m_inputMethodHints;
}

void VkbInputEngine::setInputMethodHints(Qt::InputMethodHints inputMethodHints)
{
    if (m_inputMethodHints == inputMethodHints)
        return;

    m_inputMethodHints = inputMethodHints;
    resolveInputMode();
    emit inputMethodHintsChanged();
}

Qt::KeyboardModifiers VkbInputEngine::keyboardModifiers() const
{
    return m_keyboardModifiers;
}

void VkbInputEngine::setKeyboardModifiers(Qt::KeyboardModifiers keyboardModifiers)
{
    if (m_keyboardModifiers == keyboardModifiers)
        return;

    m_keyboardModifiers = keyboardModifiers;
    resolveInputMode();
    emit keyboardModifiersChanged();
}

void VkbInputEngine::handleKeyClick(const VkbInputKey &key)
{
    // ### TODO: press & release
    typedef std::function<void(VkbInputEngine *engine, const VkbInputKey &key)> KeyHandler;
    static const QHash<Qt::Key, KeyHandler> handlers = {
        { Qt::Key_Meta, [](VkbInputEngine *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::MetaModifier); } },
        { Qt::Key_Shift, [](VkbInputEngine *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::ShiftModifier); } },
        { Qt::Key_Control, [](VkbInputEngine *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::ControlModifier); } },
        { Qt::Key_Alt, [](VkbInputEngine *engine, const VkbInputKey &) { engine->toggleKeyboardModifier(Qt::AltModifier); } }
    };

    KeyHandler handler = handlers.value(key.key, [=](VkbInputEngine *engine, const VkbInputKey &key) { engine->sendKey(key); });
    if (handler)
        handler(this, key);
}

void VkbInputEngine::resolveInputMode()
{
    if (m_inputMethodHints & Qt::ImhDigitsOnly)
        setInputMode(Digits);
    else if (m_keyboardModifiers & Qt::MetaModifier)
        setInputMode(Symbols);
    else if (m_keyboardModifiers & Qt::ShiftModifier)
        setInputMode(Capitals);
    else
        setInputMode(Letters);
}

void VkbInputEngine::sendKey(const VkbInputKey &key)
{
    if (key.key != Qt::Key_unknown) {
        QWindow *window = QGuiApplication::focusWindow();
        QWindowSystemInterface::handleKeyEvent(window, QEvent::KeyPress, key.key, m_keyboardModifiers);
        QWindowSystemInterface::handleKeyEvent(window, QEvent::KeyRelease, key.key, m_keyboardModifiers);
    } else {
        QInputMethodEvent event;
        event.setCommitString(key.text);
        QCoreApplication::sendEvent(QGuiApplication::focusObject(), &event);
    }
}

void VkbInputEngine::toggleKeyboardModifier(Qt::KeyboardModifier modifier)
{
    setKeyboardModifiers(m_keyboardModifiers ^ modifier);
}
