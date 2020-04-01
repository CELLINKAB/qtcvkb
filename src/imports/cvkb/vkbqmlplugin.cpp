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
#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>
#include <QtQuickControls2/qquickstyle.h>
#include <QtQuickControls2/private/qquickstyleselector_p.h>
#include <QtCVkb/vkbstylehints.h>
#include <QtCVkb/vkbinputcontext.h>

#include "vkbinputdelegate.h"
#include "vkbinputlayoutattached.h"
#include "vkbinputpanel.h"
#include "vkbinputpopup.h"

static const int MajorVersion = 0;
static const int MinorVersion = 1;

class VkbQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
    void initializeEngine(QQmlEngine *engine, const char *uri) override;

private:
    void registerTemplates(const char *uri, int major, int minor);
    void registerRevisions(const char *uri, int major, int minor);
    void registerStyles(const char *uri, int major, int minor);
};

void VkbQmlPlugin::registerTypes(const char *uri)
{
    const QByteArray tmpl = QByteArray(uri) + ".Templates";
    registerTemplates(tmpl, MajorVersion, MinorVersion);
    registerRevisions(tmpl, MajorVersion, MinorVersion);
    registerStyles(uri, MajorVersion, MinorVersion);
}

static inline QByteArray formatImport(const char *uri, int majorVersion, int minorVersion)
{
    return "import " + QByteArray(uri) + " " + QByteArray::number(majorVersion) + "." + QByteArray::number(minorVersion);
}

static inline QByteArray formatType(const char *typeName)
{
    return QByteArray(typeName) + " {}";
}

void VkbQmlPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    const QByteArray qml = formatImport(uri, MajorVersion, MinorVersion) + ";" + formatType("InputPanel");

    VkbInputContext *inputContext = VkbInputContext::instance();
    if (!inputContext)
        return;

    inputContext->setInputPanelFactory([=](QObject *parent) {
        VkbInputPanel *inputPanel = nullptr;

        QQmlComponent component(engine);
        component.setData(qml, QUrl());

        QQmlContext *creationContext = qmlContext(parent);
        QQmlContext *context = new QQmlContext(creationContext, parent);
        context->setContextObject(parent);

        QObject *object = component.beginCreate(context);
        if (Q_UNLIKELY(!object))
            qWarning() << component.errorString();

        inputPanel = qobject_cast<VkbInputPanel *>(object);
        if (Q_UNLIKELY(!inputPanel))
            qWarning() << "QML InputPanel is not an instance of VkbInputPanel";
        else
            inputPanel->setParent(parent);

        component.completeCreate();
        return inputPanel;
    });
}

void VkbQmlPlugin::registerTemplates(const char *uri, int majorVersion, int minorVersion)
{
    qmlRegisterType<VkbInputDelegate>(uri, majorVersion, minorVersion, "InputDelegate");
    qmlRegisterUncreatableType<VkbInputLayoutAttached>(uri, majorVersion, minorVersion, "InputLayout", QStringLiteral("InputLayout is an attached property"));
    qmlRegisterType<VkbInputPanel>(uri, majorVersion, minorVersion, "InputPanel");
    qmlRegisterType<VkbInputPopup>(uri, majorVersion, minorVersion, "InputPopup");
    qmlRegisterType<VkbStyleHints>(uri, majorVersion, minorVersion, "StyleHints");
}

void VkbQmlPlugin::registerRevisions(const char *uri, int majorVersion, int minorVersion)
{
    qmlRegisterRevision<QQuickPopup, QT_VERSION_MINOR>(uri, majorVersion, minorVersion);
}

static const QString importPath(const char *uri)
{
    return QStringLiteral(":/qt-project.org/imports/%1").arg(QString::fromLatin1(uri).replace(QLatin1Char('.'), QLatin1Char('/')));
}

void VkbQmlPlugin::registerStyles(const char *uri, int majorVersion, int minorVersion)
{
    QQuickStyleSelector selector;
    const QString style = QQuickStyle::name();
    if (!style.isEmpty())
        selector.addSelector(style);
    selector.setPaths(QQuickStyle::stylePathList() << importPath(uri));

    qmlRegisterType(selector.select(QStringLiteral("InputDelegate.qml")), uri, majorVersion, minorVersion, "InputDelegate");
    qmlRegisterType(selector.select(QStringLiteral("InputKey.qml")), uri, majorVersion, minorVersion, "InputKey");
    qmlRegisterUncreatableType<VkbInputLayoutAttached>(uri, majorVersion, minorVersion, "InputLayout", QStringLiteral("InputLayout is an attached property"));
    qmlRegisterType(selector.select(QStringLiteral("InputPanel.qml")), uri, majorVersion, minorVersion, "InputPanel");
    qmlRegisterType(selector.select(QStringLiteral("InputPopup.qml")), uri, majorVersion, minorVersion, "InputPopup");
    qmlRegisterSingletonType(selector.select(QStringLiteral("StyleHints.qml")), uri, majorVersion, minorVersion, "StyleHints");
}

#include "vkbqmlplugin.moc"
