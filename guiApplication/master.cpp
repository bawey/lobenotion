#include "master.h"
#include <QDebug>

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

    sessionsModel = new SessionsModel();
    classifiersModel = new ClassifiersModel();

    octaveProxy = new OctaveProxy();

    daq->start();

    connectModules();
}

void Master::connectModules(){
    connect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaProcessor, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
    connect(metaProcessor, SIGNAL(signalFine(bool)), spellerController, SLOT(slotSignalFine(bool)));
    spellerController->slotSignalFine(metaProcessor->signalFine());

    connect(spellerController, SIGNAL(requestPeriodClassification(QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >)),
            classifiersModel, SLOT(slotAskCurrentClassifier(QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >)));

    connect(classifiersModel, SIGNAL(signalSymbolClassified(int,int,float)), spellerController,
            SLOT(slotSymbolRecognized(int,int,float)));

    connect(octaveProxy, SIGNAL(signalOctaveError(QString)), this, SLOT(slotErrorRelay(QString)));
}

Master* Master::getInstance(){
    if(Master::instance==NULL){
        Master::mutex.lock();
        if(Master::instance==NULL){
            Master::instance=new Master();
        }
        Master::instance->doGlobalInitialization();
        Master::mutex.unlock();
    }
    return Master::instance;
}

void Master::slotErrorRelay(QString errmsg){
    emit signalErrorRelay(errmsg);
}
