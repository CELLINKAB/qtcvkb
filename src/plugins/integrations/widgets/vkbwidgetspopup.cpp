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

#include "vkbwidgetspopup.h"

#include <QtGui/qevent.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qshortcut.h>

VkbWidgetsPopup::VkbWidgetsPopup(QObject *parent)
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

void VkbWidgetsPopup::setAlt(const QStringList &alt)
{
    if (m_alt == alt)
        return;

    m_alt = alt;
}

void VkbWidgetsPopup::show()
{
    QWidget::show();
    updateButtons();
    grabMouse();
}

void VkbWidgetsPopup::hide()
{
    QWidget::close();
}

QWidget *childAt(QWidget *parent, const QPoint &globalPos)
{
    QPointF pos = parent->mapFromGlobal(globalPos);
    return parent->childAt(pos.x(), pos.y());
}

QAbstractButton *buttonAt(QWidget *parent, const QPoint &globalPos)
{
    QWidget *child = childAt(parent, globalPos);
    while (child) {
        QAbstractButton *button = qobject_cast<QAbstractButton *>(child);
        if (button)
            return button;

        child = childAt(child, globalPos);
    }
    return nullptr;
}

void VkbWidgetsPopup::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    updateCurrentButton(buttonAt(this, event->globalPos()));
    handlePress(m_currentButton);
}

void VkbWidgetsPopup::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    QAbstractButton *oldCurrent = m_currentButton;
    updateCurrentButton(buttonAt(this, event->globalPos()));
    if (m_currentButton != oldCurrent) {
        handleCancel(oldCurrent);
        handlePress(m_currentButton);
    }
}

void VkbWidgetsPopup::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    handleRelease(m_currentButton);
    updateCurrentButton(nullptr);
    close();
}

void VkbWidgetsPopup::handlePress(QAbstractButton *button)
{
    if (!button)
        return;

    VkbInputKey inputKey = button->property("_vkb_input_key").value<VkbInputKey>();
    emit keyPressed(inputKey);
}

void VkbWidgetsPopup::handleRelease(QAbstractButton *button)
{
    if (!button)
        return;

    VkbInputKey inputKey = button->property("_vkb_input_key").value<VkbInputKey>();
    emit keyReleased(inputKey);
}

void VkbWidgetsPopup::handleCancel(QAbstractButton *button)
{
    if (!button)
        return;

    VkbInputKey inputKey = button->property("_vkb_input_key").value<VkbInputKey>();
    emit keyCanceled(inputKey);
}

void VkbWidgetsPopup::updateButtons()
{
    QHash<VkbInputKey, QAbstractButton *> newButtons;
    QHash<VkbInputKey, QAbstractButton *> oldButtons = m_buttons;

    QHBoxLayout *boxLayout = static_cast<QHBoxLayout *>(QWidget::layout());
    if (!boxLayout)
        boxLayout = new QHBoxLayout(this);

    QVector<VkbInputKey> row;
    for (const QString &text : qAsConst(m_alt)) {
        VkbInputKey key(text);
        QAbstractButton *button = oldButtons.take(key);
        if (!button) {
            button = new QPushButton(text, this);
            button->setObjectName(text);
            button->setProperty("_vkb_input_key", QVariant::fromValue(key));
            boxLayout->addWidget(button);
        }
        newButtons.insert(key, button);
        row += key;
    }

    for (QAbstractButton *button : qAsConst(oldButtons)) {
        boxLayout->removeWidget(button);
        button->deleteLater();
    }
    m_buttons = newButtons;
}

void VkbWidgetsPopup::updateCurrentButton(QAbstractButton *button)
{
    if (m_currentButton == button)
        return;

    if (m_currentButton)
        m_currentButton->setDown(false);
    if (button)
        button->setDown(true);

    m_currentButton = button;
}
