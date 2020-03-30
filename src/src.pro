TEMPLATE = subdirs
SUBDIRS += \
    cvkb \
    imports \
    plugins

imports.depends = cvkb
plugins.depends = cvkb
