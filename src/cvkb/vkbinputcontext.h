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

#ifndef VKBINPUTCONTEXT_H
#define VKBINPUTCONTEXT_H

#include <QtCVkb/vkbglobal.h>
#include <QtGui/qpa/qplatforminputcontext.h>
#include <QtCore/qscopedpointer.h>
#include <functional>

class VkbInputContextPrivate;

typedef std::function<QObject *(QObject *)> VkbInputPanelFactory;

class Q_CVKB_EXPORT VkbInputContext : public QPlatformInputContext
{
    Q_OBJECT

public:
    explicit VkbInputContext(const QStringList &params = QStringList());
    ~VkbInputContext();

    static VkbInputContext *instance();

    VkbInputPanelFactory inputPanelFactory() const;
    void setInputPanelFactory(VkbInputPanelFactory inputPanelFactory);

    bool isValid() const override;
    bool hasCapability(Capability capability) const override;

    void reset() override;
    void commit() override;
    void update(Qt::InputMethodQueries queries) override;
    void invokeAction(QInputMethod::Action action, int cursorPosition) override;
    bool filterEvent(const QEvent *event) override;
    QRectF keyboardRect() const override;

    bool isAnimating() const override;

    void showInputPanel() override;
    void hideInputPanel() override;
    bool isInputPanelVisible() const override;

    QLocale locale() const override;
    Qt::LayoutDirection inputDirection() const override;

    void setFocusObject(QObject *focusObject) override;

private:
    Q_DECLARE_PRIVATE(VkbInputContext)
    QScopedPointer<VkbInputContextPrivate> d_ptr;

    // ### TODO: mark QPlatformInputContext::emitXxx() as slots
    Q_PRIVATE_SLOT(d_func(), void _q_emitInputPanelVisibleChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_emitAnimatingChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_emitKeyboardRectChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_emitLocaleChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_emitInputDirectionChanged())
};

#endif // VKBINPUTCONTEXT_H
