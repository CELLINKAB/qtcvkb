TARGET = QtCVkb
MODULE = cvkb

QT += quick
QT_PRIVATE += core-private gui-private qml-private quick-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += \
    $$PWD/vkbinputglobal.h

RESOURCES += \
    $$PWD/cvkb.qrc

include(cvkb.pri)
load(qt_module)
