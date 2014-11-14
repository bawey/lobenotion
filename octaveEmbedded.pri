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
