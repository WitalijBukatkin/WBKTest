# WBK TEST 2.0

QT       += core gui widgets

TARGET = wbktest
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

macos:ICON = res/icon.icns
win32:RC_FILE = res/icon.rc

SOURCES += \
        src/main.cpp

HEADERS += \
        src/wbktest.h \
        src/ngdialog.h \
        src/edittest.h \
        src/fragen.h \
        src/starttest.h

RESOURCES += \
    res/image.qrc
