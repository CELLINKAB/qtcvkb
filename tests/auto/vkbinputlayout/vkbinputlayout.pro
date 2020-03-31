CONFIG += testcase
QT += cvkb testlib

SOURCES += \
    tst_vkbinputlayout.cpp

TESTDATA = \
    $$files(data/*.json)

OTHER_FILES += \
    $$files(data/*.json)
