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

#ifndef VKBWIDGETSEDITOR_H
#define VKBWIDGETSEDITOR_H

#include <QtCore/qbasictimer.h>
#include <QtCore/qobject.h>
#include <QtCore/qpoint.h>
#include <QtCVkb/vkbinputeditor.h>

class VkbWidgetsEditor : public QObject, public VkbInputEditor
{
    Q_OBJECT
    Q_INTERFACES(VkbInputEditor)

public:
    explicit VkbWidgetsEditor(QObject *parent = nullptr);

    int cursorPositionAt(const QPointF &pos) const override;
    void setCursorPosition(int cursorPosition) override;

    void selectWord() override;

signals:
    void pressed(const QPointF &pos) override;
    void released(const QPointF &pos) override;
    void pressAndHold(const QPointF &pos) override;

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    void handlePress(const QPointF &pos);
    void handleMove(const QPointF &pos);
    void handleRelease(const QPointF &pos);

    QPointF m_pressPoint;
    int m_pressAndHoldInterval = -1;
    QBasicTimer m_pressAndHoldTimer;
};

#endif // VKBWIDGETSEDITOR_H
