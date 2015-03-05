#include "master.h"
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <QCoreApplication>
#include <QFileInfo>

Master* Master::instance=NULL;
QMutex Master::mutex;

Master::Master(QObject *parent) :
    QObject(parent)
{
    instantiateSettings();

    spellerController = new SpellerController(Settings::getSpellerMatrixSize(), Settings::getSpellerCharset());
    metaProcessor = new MetaProcessor();
    workerThread = new QThread();
    metaProcessor->moveToThread(workerThread);
    sessionsModel = new SessionsModel();
    classifiersModel = new ClassifiersModel();
    octaveProxy = new OctaveProxy();
    connectModules();
    workerThread->start();
}

Master::~Master(){
    if(daq!=NULL){
        daq->deleteLater();
    }
}

void Master::connectModules(){

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

void Master::instantiateSettings(){
    QStringList args = QCoreApplication::instance()->arguments();
    QString configPath = NULL;
    for(int i=0; i<args.length(); ++i){
        if(args.at(i)=="--config" && i<args.length()-1){
            configPath = args.at(i+1);
            break;
        }
    }
    while (!QFileInfo(configPath).isReadable()){
        qDebug()<<"File at "<<configPath<<" unreadanble";
        configPath = QFileDialog::getOpenFileName(0, "Pick a config file.");
    }
    Settings::instantiate(configPath);
}

void Master::slotConfigChanged(QString property){
    if(property == Settings::OPT_DUMMY_DAQ){
        recreateDaq();
    }
}

void Master::recreateDaq(){
    EegDaq* oldDaq = daq;

    if(oldDaq!=NULL){
        disconnect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaProcessor, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
        disconnect(metaProcessor, SIGNAL(signalFine(bool)), spellerController, SLOT(slotSignalFine(bool)));
        disconnect(this, SIGNAL(signalLaunchDaq()), daq, SLOT(slotLaunch()));
    }

    if(Settings::isDummyModeEnabled()){
        qDebug("Dummy mode enabled");
        daq=new FakeDaq();
    }else{
        qDebug("Dummy mode disabled");
        daq=new EpocDaq();
    }
    daq->moveToThread(workerThread);

    qDebug("DAQ started");
    connect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaProcessor, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
    connect(metaProcessor, SIGNAL(signalFine(bool)), spellerController, SLOT(slotSignalFine(bool)));
    connect(this, SIGNAL(signalLaunchDaq()), daq, SLOT(slotLaunch()), Qt::ConnectionType::QueuedConnection);
    if(oldDaq != NULL){
        oldDaq->shutdown();
        oldDaq->deleteLater();
    }
    emit signalNewDaq(daq);
    emit signalLaunchDaq();
    if(!workerThread->isRunning()){
        qDebug()<<"Worker thread no longer running";
        workerThread->start();
    }
}


void Master::start(){
    if(started == false){
       recreateDaq();
       started=true;
    }
}
