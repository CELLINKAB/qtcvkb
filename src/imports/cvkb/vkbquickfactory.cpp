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

#include "vkbquickfactory.h"

#include <QtCore/qdebug.h>
#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>

void VkbQuickFactory::init(const QByteArray &uri, int majorVersion, int minorVersion, QQmlEngine *engine)
{
    m_uri = uri;
    m_majorVersion = majorVersion;
    m_minorVersion = minorVersion;
    m_engine = engine;
}

QObject *VkbQuickFactory::createInputPanel(QObject *parent)
{
    return createInputObject("InputPanel", parent);
}

static QByteArray formatQml(const QByteArray &uri, int majorVersion, int minorVersion, const QByteArray &typeName)
{
    const QString qml = QStringLiteral("import %1 %2.%3; %4 {}");
    return qml.arg(QString::fromLatin1(uri)).arg(majorVersion).arg(minorVersion).arg(QString::fromLatin1(typeName)).toLatin1();
}

QObject *VkbQuickFactory::createInputObject(const QByteArray &typeName, QObject *parent)
{
    if (!m_engine || m_uri.isEmpty())
        return nullptr;

    QQmlComponent component(m_engine);
    component.setData(formatQml(m_uri, m_majorVersion, m_minorVersion, typeName), QUrl());

    QQmlContext *creationContext = qmlContext(parent);
    QQmlContext *context = new QQmlContext(creationContext, parent);
    context->setContextObject(parent);

    QObject *object = component.beginCreate(context);
    if (Q_UNLIKELY(!object))
        qWarning() << component.errorString();
    else
        object->setParent(parent);

    component.completeCreate();
    return object;
}
