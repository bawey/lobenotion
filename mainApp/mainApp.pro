QT  += core gui printsupport network widgets
TEMPLATE = app
SOURCES = main.cpp
include(../defaults.pri)

TARGET = ../bin/lobenotion

LIBS += -L/usr/local/lib
LIBS += -L/opt/local/lib
LIBS += -L/usr/lib64
LIBS += -lhidapi-hidraw
LIBS += -lhidapi-libusb
LIBS += -lusb-1.0
LIBS += -lmcrypt

LIBS += -L../bin -lemokit
LIBS += -L../bin -lqcustomplot
LIBS += -L../bin -lln-octave
LIBS += -L../bin -llobenotion-core
LIBS += -L../bin -llobenotion-gui

LIBS += -L/usr/local/lib -lemokit
LIBS += -L/usr/lib -lmcrypt
LIBS += -L/usr/local/lib/ -lhidapi-libusb
LIBS += -L/usr/local/lib/ -lhidapi-hidraw
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lusb-1.0

unix:!macx:!symbian: LIBS += -L/usr/lib/octave-3.2.4/ -loctave
unix:!macx:!symbian: LIBS += -L/usr/lib/octave-3.2.4/ -loctinterp
