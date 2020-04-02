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
#include "vkbinputfactory.h"
#include "vkbinputlayout.h"

class VkbNullInputPanel : public VkbInputPanel
{
public:
    bool isVisible() const override { return false; }
    void setVisible(bool) override { }
    bool isAnimating() const override { return false; }
    QRectF rect() const override { return QRectF(); }
    QLocale locale() const override { return QLocale(); }
    Qt::LayoutDirection inputDirection() const override { return Qt::LeftToRight; }
    VkbInputLayout layout() const override { return VkbInputLayout(); }
    void setLayout(const VkbInputLayout &) override { }
    void visibleChanged() override { }
    void animatingChanged() override { }
    void rectChanged() override { }
    void localeChanged() override { }
    void inputDirectionChanged() override { }
    void layoutChanged() override { }
    void keyPressed(const VkbInputKey &) override { }
    void keyReleased(const VkbInputKey &) override { }
    void keyCanceled(const VkbInputKey &) override { }
    void keyPressAndHold(const VkbInputKey &) override { }
};

VkbInputPanel *VkbInputContextPrivate::inputPanel() const
{
    VkbInputPanel *inputPanel = qobject_cast<VkbInputPanel *>(inputPanelObject);
    if (!inputPanel) {
        static VkbNullInputPanel nullInputPanel;
        return &nullInputPanel;
    }
    return inputPanel;
}

VkbInputPanel *VkbInputContextPrivate::createInputPanel()
{
    Q_Q(VkbInputContext);
    if (inputPanelObject.isNull()) {
        inputPanelObject = VkbInputFactory::instance()->createInputPanel(QGuiApplication::focusWindow());
        if (inputPanelObject) {
            QObject::connect(inputPanelObject, SIGNAL(keyPressed(VkbInputKey)), &inputEngine, SLOT(handleKeyPress(VkbInputKey)));
            QObject::connect(inputPanelObject, SIGNAL(keyReleased(VkbInputKey)), &inputEngine, SLOT(handleKeyRelease(VkbInputKey)));
            QObject::connect(inputPanelObject, SIGNAL(keyCanceled(VkbInputKey)), &inputEngine, SLOT(handleKeyCancel(VkbInputKey)));
            QObject::connect(inputPanelObject, SIGNAL(keyPressAndHold(VkbInputKey)), &inputEngine, SLOT(handleKeyPressAndHold(VkbInputKey)));
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
    q->emitInputDirectionChanged(inputPanel()->inputDirection());
}
