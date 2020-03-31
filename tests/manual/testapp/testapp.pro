TEMPLATE = app
TARGET = testapp
QT += quick cvkb

SOURCES += \
    testapp.cpp

RESOURCES += \
    testapp.qml

load(qt_build_paths)

defineReplace(stringify) {
    return('"\\\"$$1\\\""')
}

DEFINES += \
    IMPORT_PATH=$$stringify($$MODULE_BASE_OUTDIR/qml) \
    PLUGIN_PATH=$$stringify($$MODULE_BASE_OUTDIR/plugins)
