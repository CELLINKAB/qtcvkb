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

#include "vkbinputselection_p.h"
#include "vkbinputcontext_p.h"
#include "vkbinputhandle.h"
#include "vkbinputstyle.h"

#include <QtCore/qtextboundaryfinder.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qinputmethod.h>
#include <QtGui/qwindow.h>

static const int Timeout = 5000;
static const Qt::InputMethodQueries SelectionQueries = Qt::ImEnabled | Qt::ImHints | Qt::ImSurroundingText;

VkbInputSelection::VkbInputSelection(QObject *parent) : QObject(parent)
{
    QInputMethod *inputMethod = QGuiApplication::inputMethod();
    connect(inputMethod, &QInputMethod::cursorRectangleChanged, this, &VkbInputSelection::updateInputCursor);
    connect(inputMethod, &QInputMethod::anchorRectangleChanged, this, &VkbInputSelection::updateInputAnchor);

    connect(&m_inputCursor, &VkbInputHandleProxy::pressed, this, &VkbInputSelection::stopIdleTimer);
    connect(&m_inputCursor, &VkbInputHandleProxy::released, this, &VkbInputSelection::startIdleTimer);
    connect(&m_inputCursor, &VkbInputHandleProxy::moved, this, &VkbInputSelection::moveInputCursor);

    connect(&m_inputAnchor, &VkbInputHandleProxy::pressed, this, &VkbInputSelection::stopIdleTimer);
    connect(&m_inputAnchor, &VkbInputHandleProxy::released, this, &VkbInputSelection::startIdleTimer);
    connect(&m_inputAnchor, &VkbInputHandleProxy::moved, this, &VkbInputSelection::moveInputAnchor);
}

bool VkbInputSelection::isEnabled() const
{
    return m_enabled;
}

void VkbInputSelection::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    updateInputCursor();
    updateInputAnchor();
}

bool VkbInputSelection::isVisible() const
{
    return m_visible;
}

void VkbInputSelection::setVisible(bool visible)
{
    if (m_visible == visible)
        return;

    m_visible = visible;
    updateInputCursor();
    updateInputAnchor();
}

void VkbInputSelection::show()
{
    setVisible(true);
    startIdleTimer();
}

void VkbInputSelection::hide()
{
    setVisible(false);
    stopIdleTimer();
}

void VkbInputSelection::update(Qt::InputMethodQueries queries)
{
    if (!(SelectionQueries & queries) || !m_focusObject)
        return;

    QInputMethodQueryEvent event(SelectionQueries);
    QCoreApplication::sendEvent(m_focusObject, &event);
    bool enabled = event.value(Qt::ImEnabled).toBool();
    QString surroundingText = event.value(Qt::ImSurroundingText).toString();
    Qt::InputMethodHints inputMethodHints = event.value(Qt::ImHints).value<Qt::InputMethodHints>();
    setEnabled(enabled && !surroundingText.isEmpty() && !inputMethodHints.testFlag(Qt::ImhNoTextHandles));
}

void VkbInputSelection::setFocusObject(QObject *focusObject)
{
    if (m_focusObject == focusObject)
        return;

    if (m_focusObject)
        m_focusObject->removeEventFilter(this);

    m_focusObject = focusObject;

    if (focusObject) {
        update(SelectionQueries);
        focusObject->installEventFilter(this);
    } else {
        setEnabled(false);
    }
}

bool VkbInputSelection::eventFilter(QObject *object, QEvent *event)
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

void VkbInputSelection::timerEvent(QTimerEvent *event)
{
    QObject::timerEvent(event);
    if (event->timerId() == m_idleTimer.timerId()) {
        hide();
    } else if (event->timerId() == m_pressAndHoldTimer.timerId()) {
        m_pressAndHoldTimer.stop();
        handlePressAndHold(m_pressPoint);
    }
}

void VkbInputSelection::handlePress(const QPointF &pos)
{
    Q_UNUSED(pos);
    startIdleTimer();
    m_pressPoint = pos;
    m_pressTimer.start();
    m_pressAndHoldTimer.start(VkbInputStyle::instance()->pressAndHoldInterval(), this);
}

void VkbInputSelection::handleMove(const QPointF &pos)
{
    Q_UNUSED(pos);
    m_pressAndHoldTimer.stop();
}

void VkbInputSelection::handleRelease(const QPointF &pos)
{
    Q_UNUSED(pos);
    m_pressAndHoldTimer.stop();
    if (m_pressTimer.elapsed() >= VkbInputStyle::instance()->pressAndHoldInterval())
        return;

    show();
}

void VkbInputSelection::handlePressAndHold(const QPointF &pos)
{
    selectWordAt(pos);
    show();
}

void VkbInputSelection::moveInputCursor(const QPointF &pos)
{
    const QPointF cursorPos(m_inputCursor.pos() + pos);
    if (m_inputAnchor.isNull())
        moveCursorAt(cursorPos);
    else
        moveSelectionAt(cursorPos, anchorRectangle().center());
}

void VkbInputSelection::moveInputAnchor(const QPointF &pos)
{
    const QPointF anchorPos(m_inputAnchor.pos() + pos - QPointF(0, m_inputAnchor.size().height()));
    moveSelectionAt(cursorRectangle().center(), anchorPos);
}

void VkbInputSelection::startIdleTimer()
{
    m_idleTimer.start(Timeout, this);
}

void VkbInputSelection::stopIdleTimer()
{
    m_idleTimer.stop();
}

QRectF VkbInputSelection::cursorRectangle()
{
    return QGuiApplication::inputMethod()->cursorRectangle();
}

QRectF VkbInputSelection::anchorRectangle()
{
    return QGuiApplication::inputMethod()->anchorRectangle();
}

bool VkbInputSelection::isInputCursorNeeded() const
{
    if (!m_enabled || !m_visible)
        return false;

    return cursorRectangle().isValid();
}

bool VkbInputSelection::isInputAnchorNeeded() const
{
    if (!m_enabled || !m_visible)
        return false;

    const QRectF cursor = cursorRectangle();
    const QRectF anchor = anchorRectangle();
    return anchor.isValid() && cursor != anchor;
}

void VkbInputSelection::showInputCursor()
{
    const QRectF cursor = cursorRectangle();
    m_inputCursor.move(QPointF(cursor.center().x() - m_inputCursor.size().width() / 2, cursor.bottom()));
    m_inputCursor.show();
}

void VkbInputSelection::showInputAnchor()
{
    const QRectF anchor = anchorRectangle();
    m_inputAnchor.move(QPointF(anchor.center().x() - m_inputAnchor.size().width() / 2, anchor.y() - m_inputAnchor.size().height()));
    m_inputAnchor.show();
}

void VkbInputSelection::hideInputCursor()
{
    m_inputCursor.hide();
}

void VkbInputSelection::hideInputAnchor()
{
    m_inputAnchor.hide();
}

void VkbInputSelection::updateInputCursor()
{
    if (isInputCursorNeeded())
        showInputCursor();
    else
        hideInputCursor();
}

void VkbInputSelection::updateInputAnchor()
{
    if (isInputAnchorNeeded())
        showInputAnchor();
    else
        hideInputAnchor();
}

int VkbInputSelection::cursorPositionAt(const QPointF &pos) const
{
    bool ok = false;
    int cursorPosition = QInputMethod::queryFocusObject(Qt::ImCursorPosition, pos).toInt(&ok);
    if (!ok)
        return -1;
    return cursorPosition;
}

void VkbInputSelection::selectWordAt(const QPointF &pos)
{
    int cursorPosition = cursorPositionAt(pos);
    if (cursorPosition == -1)
        return;

    moveCursorPosition(cursorPosition);

    QString text = QInputMethod::queryFocusObject(Qt::ImSurroundingText, QVariant()).toString();
    if (text.isEmpty())
        return;

    QTextBoundaryFinder finder(QTextBoundaryFinder::Word, text);
    finder.setPosition(cursorPosition);

    if (cursorPosition > 0 && !finder.boundaryReasons().testFlag(QTextBoundaryFinder::StartOfItem))
        finder.toPreviousBoundary();
    if (!finder.boundaryReasons().testFlag(QTextBoundaryFinder::StartOfItem))
        return;

    int start = finder.position();
    int end = finder.toNextBoundary();
    if (!finder.boundaryReasons().testFlag(QTextBoundaryFinder::EndOfItem))
        return;

    moveCursorPosition(start, end - start);
}

void VkbInputSelection::moveCursorPosition(int cursorPosition, int length)
{
    if (!m_focusObject)
        return;

    QInputMethodEvent::Attribute attribute(QInputMethodEvent::Selection, cursorPosition, length);
    QInputMethodEvent event(QString(), {attribute});
    QGuiApplication::sendEvent(m_focusObject, &event);
}

void VkbInputSelection::moveCursorAt(const QPointF &cursorPos)
{
    const QPointF localPos = QGuiApplication::inputMethod()->inputItemTransform().inverted().map(cursorPos);
    int cursorPosition = cursorPositionAt(localPos);
    if (cursorPosition != -1)
        moveCursorPosition(cursorPosition);
}

void VkbInputSelection::moveSelectionAt(const QPointF &cursorPos, const QPointF &anchorPos)
{
    VkbInputContext::setSelectionOnFocusObject(anchorPos, cursorPos);
}
