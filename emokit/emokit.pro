QT = core
TARGET = ../lib/emokit

TEMPLATE = lib
CONFIG+= staticlib
SOURCES = emokit.c # emokitd.c
HEADERS = emokit.h # emokitd.h

#LIBS += -L/usr/local/lib
#LIBS += -L/opt/local/lib
#LIBS += -L/usr/lib64
#LIBS += -lhidapi-hidraw
#LIBS += -lhidapi-libusb
#LIBS += -lusb-1.0
#LIBS += -lmcrypt
