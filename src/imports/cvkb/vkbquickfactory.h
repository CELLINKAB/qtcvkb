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

#ifndef VKBQUICKFACTORY_H
#define VKBQUICKFACTORY_H

#include <QtCore/qobject.h>
#include <QtCVkb/vkbinputfactory.h>

QT_FORWARD_DECLARE_CLASS(QQmlEngine)

class VkbQuickFactory : public VkbInputFactory
{
public:
    void init(const QByteArray &uri, int majorVersion, int minorVersion, QQmlEngine *engine);

    QObject *createInputPanel(QObject *parent) override;
    QObject *createInputEditor(QObject *parent) override;
    QObject *createInputCursor(QObject *parent) override;
    QObject *createInputAnchor(QObject *parent) override;

private:
    QObject *createInputObject(const QByteArray &typeName, QObject *parent);

    int m_majorVersion = 0;
    int m_minorVersion = 0;
    QByteArray m_uri;
    QQmlEngine *m_engine = nullptr;
};

#endif // VKBQUICKFACTORY_H