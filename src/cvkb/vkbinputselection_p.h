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

#ifndef VKBINPUTSELECTION_P_H
#define VKBINPUTSELECTION_P_H

#include <QtCVkb/vkbinputglobal.h>
#include <QtCVkb/private/vkbinputhandle_p.h>
#include <QtCore/qbasictimer.h>
#include <QtCore/qelapsedtimer.h>
#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>

class VkbInputSelection : public QObject
{
    Q_OBJECT

public:
    explicit VkbInputSelection(QObject *parent = nullptr);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool isVisible() const;
    void setVisible(bool  visible);

public slots:
    void show();
    void hide();
    void update(Qt::InputMethodQueries queries);
    void setFocusObject(QObject *focusObject);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private slots:
    void handlePress(const QPointF &pos);
    void handleMove(const QPointF &pos);
    void handleRelease(const QPointF &pos);
    void handlePressAndHold(const QPointF &pos);

    void moveInputCursor(const QPointF &pos);
    void moveInputAnchor(const QPointF &pos);
    void startIdleTimer();
    void stopIdleTimer();

private:
    static QRectF cursorRectangle();
    static QRectF anchorRectangle();
    bool isInputCursorNeeded() const;
    bool isInputAnchorNeeded() const;
    void showInputCursor();
    void showInputAnchor();
    void hideInputCursor();
    void hideInputAnchor();
    void updateInputCursor();
    void updateInputAnchor();

    int cursorPositionAt(const QPointF &pos) const;
    void selectWordAt(const QPointF &pos);
    void moveCursorPosition(int cursorPosition, int length = 0);
    void moveCursorAt(const QPointF &cursorPos);
    void moveSelectionAt(const QPointF &cursorPos, const QPointF &anchorPos);

    bool m_enabled = true;
    bool m_visible = false;
    QPointF m_pressPoint;
    QBasicTimer m_pressAndHoldTimer;
    QElapsedTimer m_pressTimer;
    QBasicTimer m_idleTimer;
    VkbInputCursorProxy m_inputCursor;
    VkbInputAnchorProxy m_inputAnchor;
    QPointer<QObject> m_focusObject = nullptr;
};

#endif // VKBINPUTSELECTION_P_H
