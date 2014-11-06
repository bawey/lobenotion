#include "master.h"

Master* Master::instance=NULL;
QMutex Master::mutex;

Master::Master(QObject *parent) :
    QObject(parent)
{
    if(Settings::isDummyModeEnabled()){
        qDebug("dummy mode enabled");
        daq=new FakeDaq();
    }else{
        qDebug("dummy mode disabled");
        daq=new EpocDaq();
    }


    qDebug("DAQ started");

    /** DAQ starting and stopping  needs to be moved into main window **/



    metaProcessor = new MetaProcessor();
    metaProcessor->moveToThread(daq);

    spellerController = new SpellerController(daq, Settings::getSpellerMatrixSize(), Settings::getSpellerCharset());

    daq->start();


}

Master* Master::getInstance(){
    if(Master::instance==NULL){
        Master::mutex.lock();
        if(Master::instance==NULL){
            Master::instance=new Master();
        }
        Master::mutex.unlock();
    }
    return Master::instance;
}
