message("common sees PWD as: ")
message($$PWD)

INCLUDEPATH += . .. guiApplication octaveEmbedded
#this shoud just be introduced by octaveEmbedded.pri, but it fails to work that way...
INCLUDEPATH += /usr/include/octave-3.8.1/ /usr/include/octave-3.8.1/octave

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
    $$PWD/guiApplication/master.cpp \
    $$PWD/guiApplication/sessionsmodel.cpp \
    $$PWD/guiApplication/classifiersmodel.cpp \
    $$PWD/octaveEmbedded/octaveoutputreader.cpp



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
    $$PWD/guiApplication/master.h \
    $$PWD/guiApplication/sessionsmodel.h \
    $$PWD/guiApplication/classifiersmodel.h \
    $$PWD/guiApplication/classifierinfo.h \
    $$PWD/octaveEmbedded/octaveoutputreader.h


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

#DESPERATE: COPY-PASTE octaveEmbedded.pri
SOURCES +=  $$PWD/octaveEmbedded/octaveproxy.cpp \
            $$PWD/octaveEmbedded/p3sessioninfo.cpp
HEADERS +=  $$PWD/octaveEmbedded/octaveproxy.h \
            $$PWD/octaveEmbedded/p3sessioninfo.h

#Modiffs - should be somehow automatically detected or so...
INCLUDEPATH += /usr/include/octave-3.8.1/ /usr/include/octave-3.8.1/octave

LIBS += -L/usr/local/lib
LIBS += -L/opt/local/lib
LIBS += -L/usr/lib64

unix:!macx:!symbian: LIBS += -L/usr/lib/octave-3.2.4/ -loctave
unix:!macx:!symbian: LIBS += -L/usr/lib/octave-3.2.4/ -loctinterp
#unix:!macx:!symbian: LIBS += -L/usr/lib/octave-3.2.4/ -lcruft


OTHER_FILES += /usr/lib/x86_64-linux-gnu/liboctinterp.so

#ALL this was a failed attempt
#this works only on unix makefiles
DISTFILES += scripts
#message($${OUT_PWD})
install_scripts.path = $$OUT_PWD
install_scripts.files = scripts
INSTALLS += install_scripts
##this works win ce only
DEPLOYMENT += install_scripts


#The following 4 lines assure the scripts are moved when compiled from qtCreator
copy_scripts.target = copy_scripts
#development time change, instead:
#copy_scripts.commands = cp $$PWD/scripts $$OUT_PWD/ -r
#do:
message('out is: '$$OUT_PWD)
copy_scripts.commands = echo "Deleting $$OUT_PWD/scripts";
copy_scripts.commands += rm $$OUT_PWD/scripts -rf;
copy_scripts.commands += cp /home/bawey/Forge/p300-octave $$OUT_PWD/scripts -r;
QMAKE_EXTRA_TARGETS += copy_scripts
POST_TARGETDEPS += copy_scripts

