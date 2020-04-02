TARGET = QtCVkb
MODULE = cvkb

QT += quick
QT_PRIVATE += core-private gui-private qml-private quick-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += \
    $$PWD/vkbinputcontext.h \
    $$PWD/vkbinputcontext_p.h \
    $$PWD/vkbinputgrid.h \
    $$PWD/vkbinputeditor.h \
    $$PWD/vkbinputengine.h \
    $$PWD/vkbinputfactory.h \
    $$PWD/vkbinputglobal.h \
    $$PWD/vkbinputhandle.h \
    $$PWD/vkbinputkey.h \
    $$PWD/vkbinputlayout.h \
    $$PWD/vkbinputnullobject_p.h \
    $$PWD/vkbinputpanel.h \
    $$PWD/vkbinputselection.h \
    $$PWD/vkbinputstyle.h

SOURCES += \
    $$PWD/vkbinputcontext.cpp \
    $$PWD/vkbinputcontext_p.cpp \
    $$PWD/vkbinputgrid.cpp \
    $$PWD/vkbinputengine.cpp \
    $$PWD/vkbinputfactory.cpp \
    $$PWD/vkbinputlayout.cpp \
    $$PWD/vkbinputselection.cpp \
    $$PWD/vkbinputstyle.cpp

RESOURCES += \
    $$PWD/cvkb.qrc

load(qt_module)
