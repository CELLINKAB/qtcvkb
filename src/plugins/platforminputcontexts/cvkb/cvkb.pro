TARGET = cvkbinputcontextplugin

QT += cvkb
QT_PRIVATE += core-private gui-private

SOURCES += \
    $$PWD/vkbinputcontextplugin.cpp

PLUGIN_TYPE = platforminputcontexts
PLUGIN_CLASS_NAME = VkbInputContextPlugin
load(qt_build_config)
load(qt_plugin)
