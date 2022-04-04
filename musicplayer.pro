#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T21:11:25
#
#-------------------------------------------------

QT       += core gui widgets
QT       += multimedia

TARGET = musicplayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    sources/debug.cpp \
        sources/main.cpp \
        sources/mainwindow.cpp \
    sources/mainwindow_actions.cpp \
    sources/settings.cpp \
    sources/about_app.cpp

HEADERS += \
        headers/mainwindow.h \
    headers/settings.h \
    headers/about_app.h

FORMS += \
        forms/mainwindow.ui \
    forms/settings.ui \
    forms/about_app.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
