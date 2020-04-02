TEMPLATE = subdirs
SUBDIRS += \
    cvkb \
    plugins

qtHaveModule(quick): SUBDIRS += imports

imports.depends = cvkb
plugins.depends = cvkb
