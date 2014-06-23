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

    //QEmokitD emokitd;

    //MainWindow w(&emokitd);
    //w.show();

   // emokitd.start();

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

    //daq->start();
    qDebug("DAQ started");

    QList<EegProcessor*> processors;
    if(Settings::isEegVisualizerEnabled()){
        processors.push_back(new EegVisualizer());
    }

    /** register signal datatypes and connect daq to processors **/
    qRegisterMetaType<QSharedPointer<EegFrame>>("QSharedPointer<EegFrame>");

    foreach(EegProcessor* processor, processors){
        QObject::connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)),
                                     processor, SLOT(eegFrame(QSharedPointer<EegFrame>)));
        processor->start();

    }

    /** start MetaProcessor **/
    qRegisterMetaType<QSharedPointer<MetaFrame>>("QSharedPointer<MetaFrame>");
    MetaProcessor* metaProcessor = new MetaProcessor();
    QObject::connect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)),
                                 metaProcessor, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
    metaProcessor->start();

    /** create a temporary and dummy stimulant **/

    Stimulant* st = new Stimulant();
    st->start();

    return a.exec();
}
