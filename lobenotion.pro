#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T19:44:28
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lobenotion
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            qemokitd.cpp \
    eegframe.cpp \
    settings.cpp \
    metaframe.cpp \
    eegdaq.cpp \
    eegprocessor.cpp \
    timer.cpp \
    epocdaq.cpp \
    fakedaq.cpp \
    eegvisualizer.cpp \
    qcustomplot/qcustomplot.cpp \
    metaprocessor.cpp \
    stimulant.cpp \
    spellerwidget.cpp \
    eegdumper.cpp \
    spellerdumper.cpp

HEADERS  += mainwindow.h \
    qemokitd.h \
    eegframe.h \
    settings.h \
    metaframe.h \
    eegdaq.h \
    eegprocessor.h \
    timer.h \
    epocdaq.h \
    fakedaq.h \
    eegvisualizer.h \
    qcustomplot/qcustomplot.h \
    metaprocessor.h \
    stimulant.h \
    spellerwidget.h \
    eegdumper.h \
    spellerdumper.h

FORMS    += mainwindow.ui

#CONNECT TO EMOKIT STUFF
SOURCES += ./emokit/emokit.c
HEADERS += ./emokit/emokit.h

#INCLUDE HIDAPI, MCRYPT, LIBUSB
LIBS += -L/usr/local/lib
LIBS += -L/opt/local/lib
LIBS += -L/usr/lib64
LIBS += -lhidapi-hidraw
LIBS += -lhidapi-libusb
LIBS += -lusb-1.0
LIBS += -lmcrypt

QMAKE_CXXFLAGS += -std=c++11
