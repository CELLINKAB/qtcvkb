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

#include "vkbquickpanel.h"
#include "vkbquickdelegate.h"
#include "vkbquicklayout.h"
#include "vkbquickmodel.h"
#include "vkbquickpopup.h"

#include <QtQuickTemplates2/private/qquickcontrol_p.h>
#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbQuickPanel::VkbQuickPanel(QObject *parent)
    : QQuickPopup(parent),
      m_model(new VkbQuickModel(this)),
      m_layout(new VkbQuickLayout(popupItem()))
{
    setClosePolicy(CloseOnEscape);
    setContentItem(m_layout);

    connect(m_model, &VkbQuickModel::delegatesChanged, this, &VkbQuickPanel::updateButtons);

    connect(this, &QQuickPopup::visibleChanged, this, &VkbQuickPanel::visibleChanged);
    connect(this, &QQuickPopup::openedChanged, this, &VkbQuickPanel::animatingChanged);
    connect(this, &QQuickPopup::visibleChanged, this, &VkbQuickPanel::animatingChanged);
    connect(this, &QQuickPopup::localeChanged, this, &VkbQuickPanel::localeChanged);

    // ### TODO: fix QQuickPopup::spacingChange()
    if (QQuickControl *control = qobject_cast<QQuickControl *>(popupItem()))
        connect(control, &QQuickControl::spacingChanged, this, &VkbQuickPanel::updateSpacing);
}

bool VkbQuickPanel::isVisible() const
{
    return QQuickPopup::isVisible();
}

void VkbQuickPanel::show()
{
    QQuickPopup::open();
}

void VkbQuickPanel::hide()
{
    QQuickPopup::close();
}

bool VkbQuickPanel::isAnimating() const
{
    return isVisible() && !isOpened();
}

QRectF VkbQuickPanel::rect() const
{
    return m_rect;
}

QLocale VkbQuickPanel::locale() const
{
    return QQuickPopup::locale();
}

Qt::LayoutDirection VkbQuickPanel::inputDirection() const
{
    return Qt::LeftToRight;
}

QObject *VkbQuickPanel::button(const VkbInputKey &key) const
{
    return m_layout->button(key);
}

void VkbQuickPanel::setLayout(const VkbInputLayout &layout)
{
    if (m_layout->inputLayout() == layout)
        return;

    m_layout->setInputLayout(layout);
    updateButtons();
}

QQmlListProperty<VkbQuickDelegate> VkbQuickPanel::delegates()
{
    return m_model->delegates();
}

void VkbQuickPanel::handleKeyPress()
{
    VkbQuickLayoutAttached *attached = VkbQuickLayout::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyPressed(attached->inputKey());
}

void VkbQuickPanel::handleKeyRelease()
{
    VkbQuickLayoutAttached *attached = VkbQuickLayout::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyReleased(attached->inputKey());
}

void VkbQuickPanel::handleKeyCancel()
{
    VkbQuickLayoutAttached *attached = VkbQuickLayout::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyCanceled(attached->inputKey());
}

void VkbQuickPanel::handleKeyPressAndHold()
{
    QQuickAbstractButton *button = qobject_cast<QQuickAbstractButton *>(sender());
    VkbQuickLayoutAttached *attached = VkbQuickLayout::qmlAttachedPropertiesObject(button);
    if (!attached)
        return;

    emit keyPressAndHold(attached->inputKey());
}

void VkbQuickPanel::componentComplete()
{
    QQuickPopup::componentComplete();
    updateButtons();
}

void VkbQuickPanel::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPopup::geometryChanged(newGeometry, oldGeometry);
    updateRect(newGeometry);
}

void VkbQuickPanel::updateRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    m_rect = rect;
    emit rectChanged();
}

void VkbQuickPanel::updateSpacing()
{
    m_layout->setSpacing(spacing());
}

void VkbQuickPanel::updateButtons()
{
    if (!isComponentComplete())
        return;

    QHash<VkbInputKey, QQuickAbstractButton *> newButtons;
    QHash<VkbInputKey, QQuickAbstractButton *> oldButtons = m_layout->buttons();

    VkbInputLayout layout = m_layout->inputLayout();
    for (int r = 0; r < layout.rowCount(); ++r) {
        const QVector<VkbInputKey> row = layout.rowAt(r);
        for (int c = 0; c < row.count(); ++c) {
            const VkbInputKey &key = row.at(c);
            QQuickAbstractButton *button = oldButtons.take(key);
            if (!button) {
                button = m_model->createButton(key, m_layout);
                connect(button, &QQuickAbstractButton::pressed, this, &VkbQuickPanel::handleKeyPress);
                connect(button, &QQuickAbstractButton::released, this, &VkbQuickPanel::handleKeyRelease);
                connect(button, &QQuickAbstractButton::canceled, this, &VkbQuickPanel::handleKeyCancel);
                connect(button, &QQuickAbstractButton::pressAndHold, this, &VkbQuickPanel::handleKeyPressAndHold);
            }
            newButtons.insert(key, button);
        }
    }

    for (QQuickAbstractButton *button : qAsConst(oldButtons))
        button->deleteLater();

    m_layout->setButtons(newButtons);
}
