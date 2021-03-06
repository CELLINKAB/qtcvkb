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

#include <QtQml/qqmlextensionplugin.h>
#include <QtQuickControls2/qquickstyle.h>
#include <QtQuickControls2/private/qquickstyleselector_p.h>
#include <QtCVkb/vkbinputstyle.h>

#include "vkbquickdelegate.h"
#include "vkbquickhandle.h"
#include "vkbquicklayout.h"
#include "vkbquickpanel.h"
#include "vkbquickpopup.h"

static const int MajorVersion = 0;
static const int MinorVersion = 1;

class VkbQuickModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;

private:
    void registerTemplates(const char *uri, int major, int minor);
    void registerRevisions(const char *uri, int major, int minor);
    void registerStyles(const char *uri, int major, int minor);
};

void VkbQuickModule::registerTypes(const char *uri)
{
    const QByteArray tmpl = QByteArray(uri) + ".Templates";
    registerTemplates(tmpl, MajorVersion, MinorVersion);
    registerRevisions(tmpl, MajorVersion, MinorVersion);
    registerStyles(uri, MajorVersion, MinorVersion);
}

void VkbQuickModule::registerTemplates(const char *uri, int majorVersion, int minorVersion)
{
    qmlRegisterType<VkbQuickDelegate>(uri, majorVersion, minorVersion, "InputDelegate");
    qmlRegisterType<VkbQuickHandle>(uri, majorVersion, minorVersion, "InputHandle");
    qmlRegisterAnonymousType<VkbQuickLayoutAttached>(uri, majorVersion);
    qmlRegisterUncreatableType<VkbQuickLayout>(uri, majorVersion, minorVersion, "InputLayout", QStringLiteral("InputLayout is an attached property"));
    qmlRegisterType<VkbQuickPanel>(uri, majorVersion, minorVersion, "InputPanel");
    qmlRegisterType<VkbQuickPopup>(uri, majorVersion, minorVersion, "InputPopup");
    qmlRegisterType<VkbInputStyle>(uri, majorVersion, minorVersion, "InputStyle");
}

void VkbQuickModule::registerRevisions(const char *uri, int majorVersion, int minorVersion)
{
    qmlRegisterRevision<QQuickPopup, QT_VERSION_MINOR>(uri, majorVersion, minorVersion);
}

static const QString importPath(const char *uri)
{
    return QStringLiteral(":/qt-project.org/imports/%1").arg(QString::fromLatin1(uri).replace(QLatin1Char('.'), QLatin1Char('/')));
}

void VkbQuickModule::registerStyles(const char *uri, int majorVersion, int minorVersion)
{
    QQuickStyleSelector selector;
    const QString style = QQuickStyle::name();
    if (!style.isEmpty())
        selector.addSelector(style);
    selector.setPaths(QQuickStyle::stylePathList() << importPath(uri));

    qmlRegisterType(selector.select(QStringLiteral("InputAnchor.qml")), uri, majorVersion, minorVersion, "InputAnchor");
    qmlRegisterType(selector.select(QStringLiteral("InputButton.qml")), uri, majorVersion, minorVersion, "InputButton");
    qmlRegisterType(selector.select(QStringLiteral("InputCursor.qml")), uri, majorVersion, minorVersion, "InputCursor");
    qmlRegisterType<VkbQuickDelegate>(uri, majorVersion, minorVersion, "InputDelegate");
    qmlRegisterAnonymousType<VkbQuickLayoutAttached>(uri, majorVersion);
    qmlRegisterUncreatableType<VkbQuickLayout>(uri, majorVersion, minorVersion, "InputLayout", QStringLiteral("InputLayout is an attached property"));
    qmlRegisterType(selector.select(QStringLiteral("InputPanel.qml")), uri, majorVersion, minorVersion, "InputPanel");
    qmlRegisterType(selector.select(QStringLiteral("InputPopup.qml")), uri, majorVersion, minorVersion, "InputPopup");
    qmlRegisterSingletonType(selector.select(QStringLiteral("InputStyle.qml")), uri, majorVersion, minorVersion, "InputStyle");
}

#include "vkbquickmodule.moc"
