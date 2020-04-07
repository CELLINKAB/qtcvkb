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

#include "vkbinputintegration.h"
#include "vkbinputintegration_p.h"
#include "vkbinputintegrationplugin.h"

#include <QtCore/private/qfactoryloader_p.h>
#include <QtCore/private/qobject_p.h>

VkbInputIntegration *VkbInputIntegrationPrivate::instance = nullptr;
#define vkbIntegration VkbInputIntegrationPrivate::instance

Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, integrationLoader, (VkbInputIntegrationPlugin_iid, QLatin1String("/cvkbintegrations"), Qt::CaseInsensitive))

static QString resolveIntegration(const QString &name)
{
    if (!name.isEmpty())
        return name;

    const QCoreApplication *app = QCoreApplication::instance();
    if (app && app->inherits("QApplication"))
        return QStringLiteral("widgets");

    return QStringLiteral("quick");
}

bool VkbInputIntegrationPrivate::load(const QStringList &params)
{
    const QString name = resolveIntegration(params.value(0));
    return qLoadPlugin<VkbInputIntegration, VkbInputIntegrationPlugin>(integrationLoader(), name, params.mid(1));
}

VkbInputIntegration::VkbInputIntegration(QObject *parent)
    : QObject(*(new VkbInputIntegrationPrivate), parent)
{
    Q_ASSERT(!vkbIntegration);
    vkbIntegration = this;
}

VkbInputIntegration::~VkbInputIntegration()
{
    vkbIntegration = nullptr;
}

bool VkbInputIntegration::isValid()
{
    return vkbIntegration;
}

VkbInputIntegration *VkbInputIntegration::instance()
{
    return vkbIntegration;
}
