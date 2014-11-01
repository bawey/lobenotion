message("common sees PWD as: ")
message($$PWD)

INCLUDEPATH += . .. guiApplication

SOURCES += $$PWD/guiApplication/qemokitd.cpp \
    $$PWD/guiApplication/eegframe.cpp \
    $$PWD/guiApplication/settings.cpp \
    $$PWD/guiApplication/metaframe.cpp \
    $$PWD/guiApplication/eegdaq.cpp \
    $$PWD/guiApplication/eegprocessor.cpp \
    $$PWD/guiApplication/timer.cpp \
    $$PWD/guiApplication/epocdaq.cpp \
    $$PWD/guiApplication/fakedaq.cpp \
    $$PWD/guiApplication/eegvisualizer.cpp \
    $$PWD/qcustomplot/qcustomplot.cpp \
    $$PWD/guiApplication/metaprocessor.cpp \
    $$PWD/guiApplication/stimulant.cpp \
    $$PWD/guiApplication/spellerwidget.cpp \
    $$PWD/guiApplication/eegdumper.cpp \
    $$PWD/guiApplication/spellerdumper.cpp \
    $$PWD/guiApplication/mainwindow.cpp \
    $$PWD/guiApplication/spellercontroller.cpp

#HEADERS  += mainwindow.h \
#    qemokitd.h \
#    eegframe.h \
#    settings.h \
#    metaframe.h \
#    eegdaq.h \
#    eegprocessor.h \
#    timer.h \
#    epocdaq.h \
#    fakedaq.h \
#    eegvisualizer.h \
#    $$PWD/qcustomplot/qcustomplot.h \
#    metaprocessor.h \
#    stimulant.h \
#    spellerwidget.h \
#    eegdumper.h \
#    spellerdumper.h

HEADERS  += $$PWD/guiApplication/mainwindow.h \
    $$PWD/guiApplication/qemokitd.h \
    $$PWD/guiApplication/eegframe.h \
    $$PWD/guiApplication/settings.h \
    $$PWD/guiApplication/metaframe.h \
    $$PWD/guiApplication/eegdaq.h \
    $$PWD/guiApplication/eegprocessor.h \
    $$PWD/guiApplication/timer.h \
    $$PWD/guiApplication/epocdaq.h \
    $$PWD/guiApplication/fakedaq.h \
    $$PWD/guiApplication/eegvisualizer.h \
    $$PWD/qcustomplot/qcustomplot.h \
    $$PWD/guiApplication/metaprocessor.h \
    $$PWD/guiApplication/stimulant.h \
    $$PWD/guiApplication/spellerwidget.h \
    $$PWD/guiApplication/eegdumper.h \
    $$PWD/guiApplication/spellerdumper.h \
    $$PWD/guiApplication/spellercontroller.h

FORMS    +=

#CONNECT TO EMOKIT STUFF
SOURCES += $$PWD/emokit/emokit.c
HEADERS += $$PWD/emokit/emokit.h

#INCLUDE HIDAPI, MCRYPT, LIBUSB
LIBS += -L/usr/local/lib
LIBS += -L/opt/local/lib
LIBS += -L/usr/lib64
LIBS += -lhidapi-hidraw
LIBS += -lhidapi-libusb
LIBS += -lusb-1.0
LIBS += -lmcrypt

QMAKE_CXXFLAGS += -std=c++11
