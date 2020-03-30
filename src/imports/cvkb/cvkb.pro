TARGET = cvkbqmlplugin
TARGETPATH = QtCellink/Vkb
IMPORT_VERSION = 0.1

QT += qml quick cvkb
QT_PRIVATE += core-private gui-private qml-private quick-private quicktemplates2-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

OTHER_FILES += \
    qmldir

SOURCES += \
    $$PWD/vkbqmlplugin.cpp

CONFIG += no_cxx_module
load(qml_plugin)
