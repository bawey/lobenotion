SRC_DIR = $$PWD

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += /usr/include/octave-3.8.1/ /usr/include/octave-3.8.1/octave
INCLUDEPATH += $$PWD/core/ $$PWD/octaveEmbedded/ $$PWD/emokit/
INCLUDEPATH += $$PWD/qcustomplot/ $$PWD/guiApplication/ $$PWD/guiApplication/widgets

DEPENDPATH += /usr/include/octave-3.8.1/ /usr/include/octave-3.8.1/octave
DEPENDPATH += $$PWD/core/ $$PWD/octaveEmbedded/ $$PWD/emokit/
DEPENDPATH += $$PWD/qcustomplot/ $$PWD/guiApplication/ $$PWD/guiApplication/widgets
DEPENDPATH += $$PWD/mainApp
