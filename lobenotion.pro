TEMPLATE    = subdirs
CONFIG      +=ordered
SUBDIRS     = emokit qcustomplot core octaveEmbedded guiApplication \
              mainApp
#SUBDIRS   += test testOctaveEmbedded tests

core.depends = octaveEmbedded emokit
guiApplication.depends = core qcustomplot
mainApp.depends = guiApplication


