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

#ifndef VKBINPUTKEY_H
#define VKBINPUTKEY_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

QT_FORWARD_DECLARE_CLASS(QQmlComponent)

class VkbInputKey : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(QQmlComponent *popup READ popup WRITE setPopup NOTIFY popupChanged)
    Q_CLASSINFO("DefaultProperty", "delegate")

public:
    explicit VkbInputKey(QObject *parent = nullptr);

    QString key() const;
    void setKey(const QString &key);

    QQmlComponent *delegate() const;
    void setDelegate(QQmlComponent *delegate);

    QQmlComponent *popup() const;
    void setPopup(QQmlComponent *popup);

signals:
    void keyChanged();
    void delegateChanged();
    void popupChanged();

private:
    QString m_key;
    QQmlComponent *m_delegate = nullptr;
    QQmlComponent *m_popup = nullptr;
};

#endif // VKBINPUTKEY_H
