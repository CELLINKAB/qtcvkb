TARGET = cvkbquickmodule
TARGETPATH = QtCellink/Vkb

QT += qml quick cvkb
QT_PRIVATE += core-private gui-private qml-private quick-private quicktemplates2-private quickcontrols2-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

QML_FILES += \
    $$PWD/InputAnchor.qml \
    $$PWD/InputButton.qml \
    $$PWD/InputCursor.qml \
    $$PWD/InputPanel.qml \
    $$PWD/InputPopup.qml \
    $$PWD/InputStyle.qml

OTHER_FILES += \
    $$PWD/qmldir

HEADERS += \
    $$PWD/vkbquickdelegate.h \
    $$PWD/vkbquickhandle.h \
    $$PWD/vkbquicklayout.h \
    $$PWD/vkbquickmodel.h \
    $$PWD/vkbquickpanel.h \
    $$PWD/vkbquickpopup.h

SOURCES += \
    $$PWD/vkbquickdelegate.cpp \
    $$PWD/vkbquickhandle.cpp \
    $$PWD/vkbquicklayout.cpp \
    $$PWD/vkbquickmodule.cpp \
    $$PWD/vkbquickmodel.cpp \
    $$PWD/vkbquickpanel.cpp \
    $$PWD/vkbquickpopup.cpp

CONFIG += no_cxx_module builtin_resources qtquickcompiler
load(qml_plugin)
