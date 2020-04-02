TARGET = cvkbwidgetsintegrationplugin

QT += cvkb widgets

HEADERS += \
    $$PWD/vkbwidgetseditor.h \
    $$PWD/vkbwidgetsintegration.h \
    $$PWD/vkbwidgetspanel.h

SOURCES += \
    $$PWD/vkbwidgetseditor.cpp \
    $$PWD/vkbwidgetsintegration.cpp \
    $$PWD/vkbwidgetsintegrationplugin.cpp \
    $$PWD/vkbwidgetspanel.cpp

PLUGIN_TYPE = cvkbintegrations
PLUGIN_CLASS_NAME = VkbWidgetsIntegrationPlugin
load(qt_build_config)
load(qt_plugin)
