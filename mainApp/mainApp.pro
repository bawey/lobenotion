QT  += core gui printsupport network widgets
TEMPLATE = app
SOURCES = main.cpp
include(../defaults.pri)

TARGET = ../bin/lobenotion

LIBS += -L/usr/local/lib
LIBS += -L/opt/local/lib
LIBS += -L/usr/lib
LIBS += -L/usr/lib64
LIBS += -L../lib
LIBS += -L/usr/lib/x86_64-linux-gnu/

LIBS += -llobenotion-core -llobenotion-gui -lln-octave  -lqcustomplot
LIBS += -lemokit -lmcrypt -lhidapi-libusb -lhidapi-hidraw -lusb-1.0
LIBS += -loctave -loctinterp
