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

#ifndef VKBINPUTHANDLE_P_H
#define VKBINPUTHANDLE_P_H

#include <QtCVkb/vkbinputhandle.h>
#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>

class VkbInputHandleProxy : public QObject, public VkbInputHandle
{
    Q_OBJECT
    Q_INTERFACES(VkbInputHandle)

public:
    explicit VkbInputHandleProxy(QObject *parent = nullptr);

    bool isNull() const;

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
    virtual QObject *createHandle(QObject *parent) = 0;

private:
    VkbInputHandle *instance() const;
    VkbInputHandle *create();

    QPointer<QObject> m_instance;
};

class VkbInputCursorProxy : public VkbInputHandleProxy
{
public:
    explicit VkbInputCursorProxy(QObject *parent = nullptr);

protected:
    QObject *createHandle(QObject *parent) override;
};

class VkbInputAnchorProxy : public VkbInputHandleProxy
{
public:
    explicit VkbInputAnchorProxy(QObject *parent = nullptr);

protected:
    QObject *createHandle(QObject *parent) override;
};

#endif // VKBINPUTHANDLE_P_H
