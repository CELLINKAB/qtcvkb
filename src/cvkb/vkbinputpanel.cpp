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

#include "vkbinputpanel_p.h"
#include "vkbinputintegration.h"
#include "vkbinputkey.h"
#include "vkbinputpopup.h"

#include <QtCore/qlocale.h>
#include <QtCore/qrect.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qwindow.h>

class VkbInputNullPanel : public VkbInputPanel
{
public:
    bool isVisible() const override { return false; }
    void setVisible(bool) override { }
    bool isAnimating() const override { return false; }
    QRectF rect() const override { return QRectF(); }
    QLocale locale() const override { return QLocale(); }
    Qt::LayoutDirection inputDirection() const override { return Qt::LeftToRight; }
    QObject *button(const VkbInputKey &) const override { return nullptr; }
    void setLayout(const VkbInputLayout &) override { }
    void visibleChanged() override { }
    void animatingChanged() override { }
    void rectChanged() override { }
    void localeChanged() override { }
    void inputDirectionChanged(Qt::LayoutDirection) override { }
    void keyPressed(const VkbInputKey &) override { }
    void keyReleased(const VkbInputKey &) override { }
    void keyCanceled(const VkbInputKey &) override { }
    void keyPressAndHold(const VkbInputKey &) override { }
};

VkbInputPanelProxy::VkbInputPanelProxy(QObject *parent) : QObject(parent)
{
}

bool VkbInputPanelProxy::isVisible() const
{
    return instance()->isVisible();
}

void VkbInputPanelProxy::setVisible(bool visible)
{
    create()->setVisible(visible);
}

bool VkbInputPanelProxy::isAnimating() const
{
    return instance()->isAnimating();
}

QRectF VkbInputPanelProxy::rect() const
{
    return instance()->rect();
}

QLocale VkbInputPanelProxy::locale() const
{
    return instance()->locale();
}

Qt::LayoutDirection VkbInputPanelProxy::inputDirection() const
{
    return instance()->inputDirection();
}

QObject *VkbInputPanelProxy::button(const VkbInputKey &key) const
{
    return instance()->button(key);
}

void VkbInputPanelProxy::setLayout(const VkbInputLayout &layout)
{
    create()->setLayout(layout);
}

static VkbInputPopup *createPopup(const VkbInputKey &key, VkbInputPanelProxy *inputPanel)
{
    QObject *button = inputPanel->button(key);
    if (!button)
        return nullptr;

    QObject *inputPopup = VkbInputIntegration::instance()->createInputPopup(button);
    if (inputPopup) {
        QObject::connect(inputPopup, SIGNAL(keyPressed(VkbInputKey)), inputPanel, SIGNAL(keyPressed(VkbInputKey)));
        QObject::connect(inputPopup, SIGNAL(keyReleased(VkbInputKey)), inputPanel, SIGNAL(keyReleased(VkbInputKey)));
        QObject::connect(inputPopup, SIGNAL(keyCanceled(VkbInputKey)), inputPanel, SIGNAL(keyCanceled(VkbInputKey)));
    }
    return qobject_cast<VkbInputPopup *>(inputPopup);
}

void VkbInputPanelProxy::showPopup(const VkbInputKey &key)
{
    if (key.alt.isEmpty())
        return;

    VkbInputPopup *popup = createPopup(key, this);
    if (!popup)
        return;

    popup->setAlt(key.alt);
    popup->show();
}

VkbInputPanel *VkbInputPanelProxy::create()
{
    if (!m_instance) {
        m_instance = VkbInputIntegration::instance()->createInputPanel(QGuiApplication::focusWindow());
        if (m_instance) {
            QObject::connect(m_instance, SIGNAL(keyPressed(VkbInputKey)), this, SIGNAL(keyPressed(VkbInputKey)));
            QObject::connect(m_instance, SIGNAL(keyReleased(VkbInputKey)), this, SIGNAL(keyReleased(VkbInputKey)));
            QObject::connect(m_instance, SIGNAL(keyCanceled(VkbInputKey)), this, SIGNAL(keyCanceled(VkbInputKey)));
            QObject::connect(m_instance, SIGNAL(keyPressAndHold(VkbInputKey)), this, SIGNAL(keyPressAndHold(VkbInputKey)));
            QObject::connect(m_instance, SIGNAL(visibleChanged()), this, SIGNAL(visibleChanged()));
            QObject::connect(m_instance, SIGNAL(animatingChanged()), this, SIGNAL(animatingChanged()));
            QObject::connect(m_instance, SIGNAL(rectChanged()), this, SIGNAL(rectChanged()));
            QObject::connect(m_instance, SIGNAL(localeChanged()), this, SIGNAL(localeChanged()));
            QObject::connect(m_instance, SIGNAL(inputDirectionChanged(Qt::LayoutDirection)), this, SIGNAL(inputDirectionChanged(Qt::LayoutDirection)));
        }
    }
    return instance();
}

VkbInputPanel *VkbInputPanelProxy::instance() const
{
    VkbInputPanel *inputPanel = qobject_cast<VkbInputPanel *>(m_instance);
    if (!inputPanel) {
        static VkbInputNullPanel nullPanel;
        return &nullPanel;
    }
    return inputPanel;
}
