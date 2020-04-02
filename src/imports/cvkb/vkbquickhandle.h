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

#ifndef VKBQUICKHANDLE_H
#define VKBQUICKHANDLE_H

#include <QtQuickTemplates2/private/qquickpopup_p.h>
#include <QtCVkb/vkbinputhandle.h>

class VkbQuickHandle : public QQuickPopup, public VkbInputHandle
{
    Q_OBJECT
    Q_INTERFACES(VkbInputHandle)

public:
    explicit VkbQuickHandle(QObject *parent = nullptr);

    void show() override;
    void hide() override;

    QPointF pos() const override;
    QSizeF size() const override;
    void move(const QPointF &pos) override;

signals:
    void pressed(const QPointF &pos) override;
    void released(const QPointF &pos) override;
    void moved(const QPointF &pos) override;
    void canceled() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseUngrabEvent() override;

private:
    bool m_pressed = false;
};

#endif // VKBQUICKHANDLE_H
