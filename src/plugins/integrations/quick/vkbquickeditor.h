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

#ifndef VKBQUICKEDITOR_H
#define VKBQUICKEDITOR_H

#include <QtQuickTemplates2/private/qquickpopup_p.h>
#include <QtCVkb/vkbinputeditor.h>

QT_FORWARD_DECLARE_CLASS(QQuickMouseEvent)

class VkbQuickEditor : public QObject, public VkbInputEditor
{
    Q_OBJECT
    Q_INTERFACES(VkbInputEditor)

public:
    explicit VkbQuickEditor(QObject *parent = nullptr);

    int cursorPositionAt(const QPointF &pos) const override;
    void setCursorPosition(int cursorPosition) override;

    void selectWord() override;

signals:
    void pressed(const QPointF &pos) override;
    void released(const QPointF &pos) override;
    void pressAndHold(const QPointF &pos) override;

private slots:
    void handlePress(QQuickMouseEvent *event);
    void handleRelease(QQuickMouseEvent *event);
    void handlePressAndHold(QQuickMouseEvent *event);

private:
    QQmlEngine *m_engine = nullptr;
};

#endif // VKBQUICKEDITOR_H
