include(../common.pri)

message("test sees PWD as")
message($$PWD)

INCLUDEPATH += ../guiApplication

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += testlib

TARGET = tst_testspellercontroller
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_testspellercontroller.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
