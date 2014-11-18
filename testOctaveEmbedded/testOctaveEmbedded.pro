#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T12:13:59
#
#-------------------------------------------------

include(../common.pri)
INCLUDEPATH += ../guiApplication

QT       += testlib

QT       += core gui printsupport network

TARGET = tst_testoctaveembeddedtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_testoctaveembeddedtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
