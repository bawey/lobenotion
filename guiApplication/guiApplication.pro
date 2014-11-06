QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lobenotion
TEMPLATE = app

include(../common.pri)

SOURCES += main.cpp \
    widgets/eegmetadatawidget.cpp \
    widgets/spellerwidget.cpp \
    mainwindow.cpp \
    widgets/spellercontrollerwidget.cpp \
    widgets/eegplotwidget.cpp

HEADERS += \
    widgets/eegmetadatawidget.h \
    widgets/spellerwidget.h \
    mainwindow.h \
    widgets/spellercontrollerwidget.h \
    widgets/eegplotwidget.h
