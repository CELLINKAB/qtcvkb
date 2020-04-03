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

#include "vkbwidgetspanel.h"

#include <QtCore/qlocale.h>
#include <QtGui/qevent.h>
#include <QtGui/qscreen.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qshortcut.h>
#include <QtCVkb/vkbinputlayout.h>

VkbWidgetsPanel::VkbWidgetsPanel(QObject *parent)
    : QWidget(qobject_cast<QWidget *>(parent))
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_AlwaysStackOnTop);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);

    QShortcut *shortcut = new QShortcut(this);
    shortcut->setContext(Qt::ApplicationShortcut);
    shortcut->setKey(Qt::Key_Escape);
    connect(shortcut, &QShortcut::activated, this, &QWidget::close);
}

bool VkbWidgetsPanel::isVisible() const
{
    return QWidget::isVisible();
}

void VkbWidgetsPanel::setVisible(bool visible)
{
    QWidget::setVisible(visible);
}

bool VkbWidgetsPanel::isAnimating() const
{
    return false;
}

QRectF VkbWidgetsPanel::rect() const
{
    return QWidget::rect();
}

QLocale VkbWidgetsPanel::locale() const
{
    return QWidget::locale();
}

Qt::LayoutDirection VkbWidgetsPanel::inputDirection() const
{
    return Qt::LeftToRight;
}

void VkbWidgetsPanel::setLayout(const VkbInputLayout &layout)
{
    if (m_layout == layout)
        return;

    m_layout = layout;
    updateButtons();
}

void VkbWidgetsPanel::handleKeyPress()
{
    QAbstractButton *button = qobject_cast<QAbstractButton *>(sender());
    if (!button)
        return;

    VkbInputKey inputKey = button->property("_vkb_input_key").value<VkbInputKey>();
    emit keyPressed(inputKey);
}

void VkbWidgetsPanel::handleKeyRelease()
{
    QAbstractButton *button = qobject_cast<QAbstractButton *>(sender());
    if (!button)
        return;

    VkbInputKey inputKey = button->property("_vkb_input_key").value<VkbInputKey>();
    emit keyReleased(inputKey);
}

void VkbWidgetsPanel::handleKeyCancel()
{
    // ### TODO
}

void VkbWidgetsPanel::handleKeyPressAndHold()
{
    // ### TODO
}

void VkbWidgetsPanel::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LocaleChange)
        emit localeChanged();
}

void VkbWidgetsPanel::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    emit visibleChanged();
}

void VkbWidgetsPanel::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
    emit rectChanged();
}

void VkbWidgetsPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    emit rectChanged();
}

void VkbWidgetsPanel::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    const QRect availableGeometry = screen()->availableGeometry();
    QRect rect = geometry();
    rect.moveCenter(availableGeometry.center());
    rect.moveBottom(availableGeometry.bottom());
    move(rect.topLeft());
    emit visibleChanged();
}

void VkbWidgetsPanel::updateButtons()
{
    QHash<VkbInputKey, QAbstractButton *> newButtons;
    QHash<VkbInputKey, QAbstractButton *> oldButtons = m_buttons;

    QGridLayout *gridLayout = static_cast<QGridLayout *>(QWidget::layout());
    if (!gridLayout)
        gridLayout = new QGridLayout(this);

    for (int r = 0; r < m_layout.rowCount(); ++r) {
        int cc = 0;
        const QVector<VkbInputKey> row = m_layout.rowAt(r);
        for (int c = 0; c < row.count(); ++c) {
            const VkbInputKey &key = row.at(c);
            QAbstractButton *button = oldButtons.take(key);
            if (!button) {
                button = new QPushButton(key.text, this);
                button->setAutoRepeat(key.autoRepeat);
                button->setCheckable(key.checkable);
                button->setChecked(key.checked);
                button->setObjectName(key.text);
                button->setProperty("_vkb_input_key", QVariant::fromValue(key));
                connect(button, &QAbstractButton::pressed, this, &VkbWidgetsPanel::handleKeyPress);
                connect(button, &QAbstractButton::released, this, &VkbWidgetsPanel::handleKeyRelease);
                // ### TODO: connect(button, &QAbstractButton::canceled, this, &VkbWidgetsPanel::handleKeyCancel);
                // ### TODO: connect(button, &QAbstractButton::pressAndHold, this, &VkbWidgetsPanel::handleKeyPressAndHold);
                gridLayout->addWidget(button, r, cc, 1, key.span);
            }
            newButtons.insert(key, button);
            cc += key.span;
        }
    }

    for (QAbstractButton *button : qAsConst(oldButtons)) {
        gridLayout->removeWidget(button);
        button->deleteLater();
    }
    m_buttons = newButtons;
}
