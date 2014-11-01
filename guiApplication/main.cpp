#include "mainwindow.h"
#include <QApplication>
#include "qemokitd.h"
#include <QLCDNumber>
#include "settings.h"
#include <eegdaq.h>
#include <fakedaq.h>
#include <epocdaq.h>
#include <timer.h>
#include <QList>
#include <eegprocessor.h>
#include <eegvisualizer.h>
#include <metaprocessor.h>
#include <stimulant.h>
#include <eegdumper.h>
#include <spellerdumper.h>

/**
needs to start modules:
- DAQ
- Guis
- Stimulant
- Timer
**/

int main(int argc, char *argv[])
{
    qDebug("Starting up...");
    qsrand((uint)QTime::currentTime().msec());
    QApplication a(argc, argv);

        /** start the timer **/

    Timer::start();
    qDebug("Timer started");

    /** create DAQ module **/
    EegDaq* daq=0;
    if(Settings::isDummyModeEnabled()){
        qDebug("dummy mode enabled");
        daq=new FakeDaq();
    }else{
        qDebug("dummy mode disabled");
        daq=new EpocDaq();
    }

    daq->start();
    qDebug("DAQ started");
    /** DAQ starting and stopping  needs to be moved into main window**/



    MetaProcessor* metaProcessor = new MetaProcessor();

    /** create a temporary and dummy stimulant **/
    SpellerWidget* speller = new SpellerWidget();

    /** and a no less dummy speller dumper **/
    SpellerDumper* spellDumper = new SpellerDumper();


    MainWindow* mainWindow = new MainWindow(NULL, metaProcessor, spellDumper, NULL, speller, daq);
    mainWindow->show();

    return a.exec();
}
