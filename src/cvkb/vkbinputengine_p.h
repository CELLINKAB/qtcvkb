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

#ifndef VKBINPUTENGINE_P_H
#define VKBINPUTENGINE_P_H

#include <QtCVkb/vkbinputengine.h>
#include <QtCVkb/vkbinputlayout.h>
#include <QtCore/private/qobject_p.h>

class VkbInputEnginePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(VkbInputEngine)

public:
    void resolveLayout();
    void resolveInputMode();
    void sendKeyPress(const VkbInputKey &key);
    void sendKeyRelease(const VkbInputKey &key);
    void sendKeyText(const QString &text);
    void sendKeyEvent(QEvent::Type type, const VkbInputKey &key);
    void toggleKeyboardModifier(Qt::KeyboardModifier modifier);

    VkbInputLayout layout;
    VkbInputEngine::InputMode inputMode = VkbInputEngine::Letters;
    Qt::InputMethodHints inputMethodHints = Qt::ImhNone;
    Qt::KeyboardModifiers keyboardModifiers = Qt::NoModifier;
};

#endif // VKBINPUTENGINE_P_H
