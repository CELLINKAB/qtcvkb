TARGET = cvkbqmlplugin
TARGETPATH = QtCellink/Vkb
IMPORT_VERSION = 0.1

QT += qml quick cvkb
QT_PRIVATE += core-private gui-private qml-private quick-private quicktemplates2-private quickcontrols2-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

QML_FILES += \
    $$PWD/InputPanel.qml \
    $$PWD/StyleHints.qml

OTHER_FILES += \
    $$PWD/qmldir

HEADERS += \
    $$PWD/vkbinputkey.h \
    $$PWD/vkbinputlayoutattached.h \
    $$PWD/vkbinputpanel.h

SOURCES += \
    $$PWD/vkbinputkey.cpp \
    $$PWD/vkbinputlayoutattached.cpp \
    $$PWD/vkbinputpanel.cpp \
    $$PWD/vkbqmlplugin.cpp

CONFIG += no_cxx_module builtin_resources qtquickcompiler
load(qml_plugin)
