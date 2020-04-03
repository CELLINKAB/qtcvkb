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

#ifndef VKBINPUTCONTEXT_P_H
#define VKBINPUTCONTEXT_P_H

#include <QtCVkb/vkbinputcontext.h>
#include <QtCVkb/vkbinputengine.h>
#include <QtCVkb/vkbinputpanel.h>
#include <QtCVkb/vkbinputselection.h>
#include <QtCore/qpointer.h>

class VkbInputPanel;
class VkbInputPopup;
class VkbInputIntegration;

class VkbInputContextPrivate
{
    Q_DECLARE_PUBLIC(VkbInputContext)

public:
    static VkbInputIntegration *getInputIntegration()
    {
        VkbInputContext *inputContext = VkbInputContext::instance();
        return inputContext->d_func()->inputIntegration;
    }

    void loadIntegration(const QStringList &params);

    VkbInputPanel *inputPanel() const;
    VkbInputPanel *createInputPanel();
    VkbInputPopup *createInputPopup(const VkbInputKey &key);

    bool loadInputLayout();

    void _q_handleKeyPress(const VkbInputKey &key);
    void _q_handleKeyRelease(const VkbInputKey &key);
    void _q_handleKeyCancel(const VkbInputKey &key);
    void _q_handleKeyPressAndHold(const VkbInputKey &key);

    // ### TODO: mark QPlatformInputContext::emitXxx() as slots
    void _q_emitInputPanelVisibleChanged();
    void _q_emitAnimatingChanged();
    void _q_emitKeyboardRectChanged();
    void _q_emitLocaleChanged();
    void _q_emitInputDirectionChanged();

    VkbInputContext *q_ptr = nullptr;
    QPointer<QObject> inputPanelObject;
    VkbInputEngine inputEngine;
    VkbInputSelection inputSelection;
    VkbInputIntegration *inputIntegration = nullptr;
};

#endif // VKBINPUTCONTEXT_P_H
