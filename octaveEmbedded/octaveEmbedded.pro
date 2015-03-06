QT = core
TARGET = ../lib/ln-octave
CONFIG+= staticlib

TEMPLATE = lib
SOURCES = octaveproxy.cpp p3sessioninfo.cpp octaveoutputreader.cpp
HEADERS = octaveproxy.h p3sessioninfo.h octaveoutputreader.h ClassifierOutput.h classifierinfo.h

include(../defaults.pri)
