QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lobenotion
TEMPLATE = app

#include(../octaveEmbedded.pri)
include(../common.pri)

SOURCES += main.cpp \
    widgets/eegmetadatawidget.cpp \
    widgets/spellerwidget.cpp \
    mainwindow.cpp \
    widgets/spellercontrollerwidget.cpp \
    widgets/eegplotwidget.cpp \
    widgets/sessionsmanagerwidget.cpp \
    widgets/octaveoutputwidget.cpp \
    widgets/classifiersmanagerwidget.cpp \
    widgets/settingswidget.cpp \
    widgets/classifierswidget.cpp \
    widgets/analysiswidget.cpp

HEADERS += \
    widgets/eegmetadatawidget.h \
    widgets/spellerwidget.h \
    mainwindow.h \
    widgets/spellercontrollerwidget.h \
    widgets/eegplotwidget.h \
    widgets/sessionsmanagerwidget.h \
    widgets/octaveoutputwidget.h \
    widgets/classifiersmanagerwidget.h \
    widgets/settingswidget.h \
    widgets/classifierswidget.h \
    widgets/analysiswidget.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

