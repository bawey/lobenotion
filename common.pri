message("common sees PWD as: ")
message($$PWD)

INCLUDEPATH += . .. guiApplication

SOURCES += $$PWD/guiApplication/qemokitd.cpp \
    $$PWD/guiApplication/eegframe.cpp \
    $$PWD/guiApplication/settings.cpp \
    $$PWD/guiApplication/metaframe.cpp \
    $$PWD/guiApplication/eegdaq.cpp \
    $$PWD/guiApplication/timer.cpp \
    $$PWD/guiApplication/epocdaq.cpp \
    $$PWD/guiApplication/fakedaq.cpp \
    $$PWD/qcustomplot/qcustomplot.cpp \
    $$PWD/guiApplication/metaprocessor.cpp \
    $$PWD/guiApplication/stimulant.cpp \
    $$PWD/guiApplication/spellerdumper.cpp \
    $$PWD/guiApplication/spellercontroller.cpp \
    $$PWD/guiApplication/master.cpp


#HEADERS  += mainwindow.h \
#    qemokitd.h \
#    eegframe.h \
#    settings.h \
#    metaframe.h \
#    eegdaq.h \
#    timer.h \
#    epocdaq.h \
#    fakedaq.h \
#    $$PWD/qcustomplot/qcustomplot.h \
#    metaprocessor.h \
#    stimulant.h \
#    spellerwidget.h \
#    spellerdumper.h

HEADERS += $$PWD/guiApplication/qemokitd.h \
    $$PWD/guiApplication/eegframe.h \
    $$PWD/guiApplication/settings.h \
    $$PWD/guiApplication/metaframe.h \
    $$PWD/guiApplication/eegdaq.h \
    $$PWD/guiApplication/timer.h \
    $$PWD/guiApplication/epocdaq.h \
    $$PWD/guiApplication/fakedaq.h \
    $$PWD/qcustomplot/qcustomplot.h \
    $$PWD/guiApplication/metaprocessor.h \
    $$PWD/guiApplication/stimulant.h \
    $$PWD/guiApplication/spellerdumper.h \
    $$PWD/guiApplication/spellercontroller.h \
    $$PWD/guiApplication/master.h

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
