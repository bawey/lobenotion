include(../defaults.pri)

TEMPLATE=lib
CONFIG+= staticlib
QT = core gui widgets printsupport
TARGET = ../lib/lobenotion-core

SOURCES = classifiersmodel.cpp eegdaq.cpp eegframe.cpp epocdaq.cpp fakedaq.cpp master.cpp
SOURCES+= metaframe.cpp metaprocessor.cpp sessionsmodel.cpp settings.cpp spellercontroller.cpp
SOURCES+= spellerdumper.cpp stimulant.cpp timer.cpp


HEADERS = classifiersmodel.h eegdaq.h eegframe.h epocdaq.h fakedaq.h master.h metaframe.h
HEADERS+= metaprocessor.h sessionsmodel.h settings.h spellercontroller.h spellerdumper.h
HEADERS+= timer.h stimulant.h

#LIBS += -L../bin -lemokit
#LIBS += -L../bin -lln-octave
