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

#include "vkbwidgetseditor.h"

#include <QtGui/qevent.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qstylehints.h>
#include <QtWidgets/qwidget.h>

VkbWidgetsEditor::VkbWidgetsEditor(QObject *parent)
    : QObject(parent),
      m_pressAndHoldInterval(QGuiApplication::styleHints()->mousePressAndHoldInterval())
{
    parent->installEventFilter(this);
}

bool VkbWidgetsEditor::eventFilter(QObject *object, QEvent *event)
{
    bool filter = QObject::eventFilter(object, event);
    if (!filter) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            handlePress(static_cast<QMouseEvent *>(event)->pos());
            break;
        case QEvent::MouseMove:
            m_pressAndHoldTimer.stop();
            break;
        case QEvent::MouseButtonRelease:
            emit handleRelease(static_cast<QMouseEvent *>(event)->pos());
            break;
        default:
            break;
        }
    }
    return filter;
}

void VkbWidgetsEditor::timerEvent(QTimerEvent *event)
{
    QObject::timerEvent(event);
    if (event->timerId() == m_pressAndHoldTimer.timerId()) {
        m_pressAndHoldTimer.stop();
        emit pressAndHold(m_pressPoint);
    }
}

void VkbWidgetsEditor::handlePress(const QPointF &pos)
{
    m_pressPoint = pos;
    m_pressAndHoldTimer.start(m_pressAndHoldInterval, this);
    emit pressed(pos);
}

void VkbWidgetsEditor::handleMove(const QPointF &pos)
{
    Q_UNUSED(pos);
    m_pressAndHoldTimer.stop();
}

void VkbWidgetsEditor::handleRelease(const QPointF &pos)
{
    m_pressAndHoldTimer.stop();
    emit released(pos);
}
