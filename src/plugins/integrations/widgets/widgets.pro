TARGET = cvkbwidgetsintegrationplugin

QT += cvkb widgets

HEADERS += \
    $$PWD/vkbwidgetsintegration.h \
    $$PWD/vkbwidgetspanel.h \
    $$PWD/vkbwidgetspopup.h

SOURCES += \
    $$PWD/vkbwidgetsintegration.cpp \
    $$PWD/vkbwidgetsintegrationplugin.cpp \
    $$PWD/vkbwidgetspanel.cpp \
    $$PWD/vkbwidgetspopup.cpp

PLUGIN_TYPE = cvkbintegrations
PLUGIN_CLASS_NAME = VkbWidgetsIntegrationPlugin
load(qt_build_config)
load(qt_plugin)
