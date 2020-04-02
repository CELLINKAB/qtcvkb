TARGET = cvkbquickintegrationplugin

QT += cvkb quick
QT_PRIVATE += quick-private quicktemplates2-private

HEADERS += \
    $$PWD/vkbquickeditor.h \
    $$PWD/vkbquickintegration.h

SOURCES += \
    $$PWD/vkbquickeditor.cpp \
    $$PWD/vkbquickintegration.cpp \
    $$PWD/vkbquickintegrationplugin.cpp

PLUGIN_TYPE = cvkbintegrations
PLUGIN_CLASS_NAME = VkbQuickIntegrationPlugin
load(qt_build_config)
load(qt_plugin)
