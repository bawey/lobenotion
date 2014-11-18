#QT       += core gui printsupport
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#TARGET = lobenotion
TEMPLATE = subdirs
SUBDIRS += guiApplication \
#    testOctaveEmbedded
#    test \

#this one would crawl the directories looking for the files required
DEPENDPATH += /usr/share/octave-3.8.1
