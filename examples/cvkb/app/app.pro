TEMPLATE = app
TARGET = app
QT += quick cvkb

SOURCES += \
    app.cpp

RESOURCES += \
    app.qml

target.path = $$[QT_INSTALL_EXAMPLES]/cvkb/app
INSTALLS += target
