TEMPLATE = app
TARGET = widgetstestapp
QT += widgets cvkb

SOURCES += \
    widgetstestapp.cpp

FORMS += \
    widgetstestapp.ui

load(qt_build_paths)

defineReplace(stringify) {
    return('"\\\"$$1\\\""')
}

DEFINES += \
    PLUGIN_PATH=$$stringify($$MODULE_BASE_OUTDIR/plugins)
