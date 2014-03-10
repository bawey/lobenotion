#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T19:44:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lobenotion
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            qemokitd.cpp \
    eegframe.cpp \
    eegmaintenanceframe.cpp

HEADERS  += mainwindow.h \
    qemokitd.h \
    eegframe.h \
    eegmaintenanceframe.h

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
