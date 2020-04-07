TARGET = QtCVkb
MODULE = cvkb
MODULE_PLUGIN_TYPES += cvkbintegrations

QT += quick
QT_PRIVATE += core-private gui-private qml-private quick-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += \
    $$PWD/vkbinputcontext.h \
    $$PWD/vkbinputcontext_p.h \
    $$PWD/vkbinputgrid.h \
    $$PWD/vkbinputengine.h \
    $$PWD/vkbinputengine_p.h \
    $$PWD/vkbinputglobal.h \
    $$PWD/vkbinputhandle.h \
    $$PWD/vkbinputhandle_p.h \
    $$PWD/vkbinputintegration.h \
    $$PWD/vkbinputintegration_p.h \
    $$PWD/vkbinputintegrationplugin.h \
    $$PWD/vkbinputkey.h \
    $$PWD/vkbinputlayout.h \
    $$PWD/vkbinputpanel.h \
    $$PWD/vkbinputpanel_p.h \
    $$PWD/vkbinputpopup.h \
    $$PWD/vkbinputselection.h \
    $$PWD/vkbinputstyle.h

SOURCES += \
    $$PWD/vkbinputcontext.cpp \
    $$PWD/vkbinputgrid.cpp \
    $$PWD/vkbinputengine.cpp \
    $$PWD/vkbinputhandle.cpp \
    $$PWD/vkbinputintegration.cpp \
    $$PWD/vkbinputlayout.cpp \
    $$PWD/vkbinputpanel.cpp \
    $$PWD/vkbinputselection.cpp \
    $$PWD/vkbinputstyle.cpp

RESOURCES += \
    $$PWD/cvkb.qrc

load(qt_module)
