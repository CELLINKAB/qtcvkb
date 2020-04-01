TARGET = cvkbqmlplugin
TARGETPATH = QtCellink/Vkb
IMPORT_VERSION = 0.1

QT += qml quick cvkb
QT_PRIVATE += core-private gui-private qml-private quick-private quicktemplates2-private quickcontrols2-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

QML_FILES += \
    $$PWD/InputButton.qml \
    $$PWD/InputDelegate.qml \
    $$PWD/InputPanel.qml \
    $$PWD/InputPopup.qml \
    $$PWD/StyleHints.qml

OTHER_FILES += \
    $$PWD/qmldir

HEADERS += \
    $$PWD/vkbinputdelegate.h \
    $$PWD/vkbinputlayoutattached.h \
    $$PWD/vkbinputlayoutitem.h \
    $$PWD/vkbinputpanel.h \
    $$PWD/vkbinputpopup.h

SOURCES += \
    $$PWD/vkbinputdelegate.cpp \
    $$PWD/vkbinputlayoutattached.cpp \
    $$PWD/vkbinputlayoutitem.cpp \
    $$PWD/vkbinputpanel.cpp \
    $$PWD/vkbinputpopup.cpp \
    $$PWD/vkbqmlplugin.cpp

CONFIG += no_cxx_module builtin_resources qtquickcompiler
load(qml_plugin)
