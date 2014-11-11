#include "spellercontroller.h"
#include <cstdlib>
#include <QStringList>
#include <QDebug>
#include <QTime>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <timer.h>


/**
 * @brief SpellerController::SpellerController
 * @param newMatrixSize
 * @param symbols
 * @param parent
 *
 * Scenario is strictly constrained with respect to: number of blinks, number of dims
 *
 */
SpellerController::SpellerController(EegDaq* daqPtr, unsigned short int newMatrixSize, QString symbols, QObject *parent) :
    daq(daqPtr), QObject(parent), matrixSize(newMatrixSize)
{
    this->keyboardSymbols=QVector<QString>();

    QStringList tokens = symbols.split(' ');

    for(int i=0; i<tokens.length(); ++i){
        this->keyboardSymbols.push_back(tokens.at(i));
    }

    thread = new QThread();
    thread->setObjectName("SpellerControllerThread");

    this->moveToThread(thread);
    thread->start();

    /** set the Dumper up **/
    //TODO: remove hardcoded path
    dumper = new SpellerDumper();

    dumperThread = new QThread();
    dumper->moveToThread(dumperThread);
    dumperThread->start();

    connectSignalsToSlots();
}

void SpellerController::connectSignalsToSlots(){
    //qRegisterMetaType<dataTakingParams*>("dataTakingParams*");
    connect(this, SIGNAL(commandRowColHighlight(short)), dumper, SLOT(spellerHighlight(short)));
    connect(this, SIGNAL(commandIndicateTarget(short,short)), dumper, SLOT(spellerHint(short,short)));
    connect(this, SIGNAL(dataTakingStarted(dataTakingParams*)), dumper, SLOT(startDumpingSession(dataTakingParams*)));
    connect(this, SIGNAL(dataTakingEnded()), dumper, SLOT(closeDumpingSession()));
    connect(this, SIGNAL(error(unsigned char)), dumper, SLOT(spellerError(unsigned char)));
}

SpellerController::~SpellerController(){
    disconnectSignalsFromSlots();
    //should it be auto-deleted through parentage??
    delete thread;
}

void SpellerController::disconnectSignalsFromSlots(){
    disconnect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), this, SLOT(eegMetaFrame(QSharedPointer<MetaFrame>)));
    disconnect(this, SIGNAL(commandRowColHighlight(short)), dumper, SLOT(spellerHighlight(short)));
    disconnect(this, SIGNAL(dataTakingStarted(dataTakingParams*)), dumper, SLOT(startDumpingSession(dataTakingParams*)));
    disconnect(this, SIGNAL(dataTakingEnded()), dumper, SLOT(closeDumpingSession()));
}

/**
 * Rounds up the sleep duration to a multiple of probing period.
 * Essentailly it blocks waiting for new frames until the waiting period
 * reaches the intended duration.
 *
 * Can we still dispatch frames in such situation???
 *
 * @brief SpellerController::sleepFrameAligned
 * @param duration
 */
void SpellerController::sleepFrameAligned(unsigned long duration){
    int startTime = Timer::getTime();
    QMutex mutex;
    while(Timer::getTime()-startTime<duration){
        mutex.lock();
        daq->getFrameEmittedWaitCondition()->wait(&mutex);
        mutex.unlock();
    }
//    qDebug()<<"frame aligned sleep slept for "<<(Timer::getTime()-startTime)<<" of ouf "<<duration;
}

QVector<short int>* SpellerController::blockRandomizeFlashes(){
    QVector<short int>* stimuli = new QVector<short int>();
    QVector<short int>* rowStimuli = new QVector<short int>();
    QVector<short int>* colStimuli = new QVector<short int>();

    rowStimuli->push_back(1);
    colStimuli->push_back(-1);
    for(int i=1; i<this->matrixSize; ++i){

//        DebugDetails
//        int random = qrand();
//        float ratio = (float)random/RAND_MAX;
//        int randomIndex = (int)((rowStimuli->size())*ratio+0.5);
//        qDebug()<<"inserting at "<<randomIndex<<". position of a "<<rowStimuli->length()<<"element long list. Random was "<<random<<", ratio: "<<ratio<<endl;

        rowStimuli->insert((int)((rowStimuli->size())*((float)qrand()/RAND_MAX)+0.5), i+1);
        colStimuli->insert((int)((colStimuli->size())*((float)qrand()/RAND_MAX)+0.5), -(i+1));
    }

    for(int i=0; i<matrixSize; ++i){
        stimuli->push_back(rowStimuli->at(i));
        stimuli->push_back(colStimuli->at(i));
    }

    delete rowStimuli;
    delete colStimuli;

    return stimuli;
}

void SpellerController::dataTakingJob(dataTakingParams *params){
    bool signalNeverBad = true;

    // this flag can externally be set via a signal-slot connection from the GUI
    flagAbort=false;

    if(!validateInputString(params->phrase) || !validateTimings(*params)){
        emit error(SpellerController::ERRCODE_PARAMETERS);
        return;
    }else{
        emit dataTakingStarted(params);
        connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), dumper, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    }

    for(int targetIdx = 0; targetIdx<params->phrase.length() && signalNeverBad && !flagAbort; ++targetIdx){
        QString target = params->phrase.mid(targetIdx, 1);
        unsigned short int number = keyboardSymbols.indexOf(target);
        unsigned short int row;
        unsigned short int column;
        symbolNumberToRowCol(number, row, column);

        emit commandNextPeriod();
        //qDebug()<<"Instructing to focus on column "<<column<<", row "<<row;
        emit commandIndicateTarget(row, column);

        //thread->msleep(infoDuration);
        this->sleepFrameAligned(params->stintInfo);

        // qDebug()<<"Dimming for short";
        emit commandDimKeyboard();
        //thread->msleep(interStimulusInterval);
        this->sleepFrameAligned(params->stintInterPeriod);

        // signal quality is periodically checked in some parts of this loop
        // a class member field, signalFine, can be set via a signal-slot while the loop executes
        signalNeverBad = signalNeverBad && flagSignalFine;

        for(int flashNo=0; flashNo<params->epochsPerStimulus && signalNeverBad && !flagAbort; ++flashNo){
            // qDebug()<<"Flashing everything: "<<flashNo<<"/"<<epochsPerStimulus;
            QVector<short>* stimuli = blockRandomizeFlashes();
            for(int blockNo = 0; blockNo<stimuli->length() && signalNeverBad && !flagAbort; ++blockNo){
                //  qDebug()<<"Flashing stimulus of code: "<<stimuli->at(blockNo);
                emit commandRowColHighlight(stimuli->at(blockNo));
                // thread->msleep(highlightDuration);
                this->sleepFrameAligned(params->stintHighlight);
                //  qDebug()<<"Dimming for short";
                emit commandDimKeyboard();
                //thread->msleep(interStimulusInterval-highlightDuration);
                this->sleepFrameAligned(params->stintDim);
                signalNeverBad = signalNeverBad && flagSignalFine;
            }
            delete stimuli;
        }

        // qDebug()<<QString("inter-period sleep for target %1 of %2 ...").arg(targetIdx+1).arg(phrase.size());
        emit commandDimKeyboard();
        //thread->msleep(interPeriodInterval);
        this->sleepFrameAligned(params->stintInterPeriod);
    }
    // qDebug()<<QString("### DONE in thread: %1 ###").arg(QThread::currentThread()->objectName());
    if(!signalNeverBad){
        emit error(ERRCODE_SIGNAL);
    }else if(flagAbort){
        emit error(ERRCODE_ABORTED);
    }

    // Make sure the dumper doesn't capture more frames
    disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), dumper, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    emit dataTakingEnded();
    delete params;
}

void SpellerController::startDataTaking(QString phrase, int epochsPerStimulus, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,
                                        QString subjectName, QString parentDirectory){
    dataTakingParams* params = new dataTakingParams();

    params->stintHighlight=highlightDuration;
    params->stintDim=interStimulusInterval-highlightDuration;
    params->stintInfo=infoDuration;
    params->stintInterPeriod=interPeriodInterval;
    params->epochsPerStimulus=epochsPerStimulus;
    params->subjectName=subjectName;
    params->parentDir=parentDirectory;
    params->phrase=phrase;

    QtConcurrent::run(this, &SpellerController::dataTakingJob, params);
}

void SpellerController::endDataTaking(){
    qDebug()<<"data taking abort request received";
    flagAbort=true;
}

void SpellerController::startOnline(int epochsPerStimulus, int interStimulusInterval, int highlightDuration, int infoDuration){}

void SpellerController::endOnline(){}

void SpellerController::slotSignalFine(bool isIt){
    this->flagSignalFine=isIt;
}


