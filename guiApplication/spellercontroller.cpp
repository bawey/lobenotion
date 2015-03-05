#include "spellercontroller.h"
#include <cstdlib>
#include <QStringList>
#include <QDebug>
#include <QTime>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <timer.h>
#include <climits>
#include <master.h>


/**
 * @brief SpellerController::SpellerController
 * @param newMatrixSize
 * @param symbols
 * @param parent
 *
 * Scenario is strictly constrained with respect to: number of blinks, number of dims
 *
 */
SpellerController::SpellerController(unsigned short int newMatrixSize, QString symbols, QObject *parent) :
    QObject(parent), matrixSize(newMatrixSize)
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

    connect(this, SIGNAL(onlineModeStarted(const dataTakingParams*)), dumper, SLOT(startOnlineMode(const dataTakingParams*)));
    connect(this, SIGNAL(onlineModeEnded()), dumper, SLOT(closeOnlineMode()));
    connect(this, SIGNAL(onlinePeriodStarted()), dumper, SLOT(startOnlinePeriod()));
    connect(this, SIGNAL(onlinePeriodEnded()), dumper, SLOT(closeOnlinePeriod()));
    connect(this, SIGNAL(signalOnlineEpochEnded()), dumper, SLOT(slotOnlineEpochEnded()));

    connect(dumper, SIGNAL(onlinePeriodCaptured(QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >)),
            this, SLOT(slotCapturedOnlinePeriod(QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >)));

    connect(dumper, SIGNAL(signalOnlineEpochCaptured(QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >)),
            this, SLOT(slotCapturedOnlineEpoch(QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >,QSharedPointer<QVector<int> >)));

}

SpellerController::~SpellerController(){
    disconnectSignalsFromSlots();
    //should it be auto-deleted through parentage??
    delete thread;
}

void SpellerController::disconnectSignalsFromSlots(){
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
    while((unsigned long)(Timer::getTime()-startTime) < duration){
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

    if(!params->isOnline && !validateInputString(params->phrase)){
        emit error(SpellerController::ERRCODE_PHRASE);
        return;
    }else if(!validateTimings(*params)){
        emit error(SpellerController::ERRCODE_PARAMETERS);
        return;
    }else{
        if(params->isOnline){
            emit onlineModeStarted(params);
        }else{
            emit dataTakingStarted(params);
        }
        connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), dumper, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    }

    int symbolsLimit = params->isOnline ? params->symbolsLimit: params->phrase.length();

    for(int targetIdx = 0; (symbolsLimit == 0 || targetIdx<symbolsLimit) && signalNeverBad && !flagAbort; ++targetIdx){
        emit commandNextPeriod();
        if(params->isOnline){
            emit commandShowMessage("Get ready for the next character!");
        }else{
            QString target = params->phrase.mid(targetIdx, 1);
            unsigned short int number = keyboardSymbols.indexOf(target);
            unsigned short int row;
            unsigned short int column;
            symbolNumberToRowCol(number, row, column);

            //qDebug()<<"Instructing to focus on column "<<column<<", row "<<row;
            emit commandIndicateTarget(row, column);
        }

        //thread->msleep(infoDuration);
        this->sleepFrameAligned(params->stintInfo);

        // qDebug()<<"Dimming for short";
        emit commandDimKeyboard();
        //thread->msleep(interStimulusInterval);
        this->sleepFrameAligned(params->stintInterPeriod);

        // signal quality is periodically checked in some parts of this loop
        // a class member field, signalFine, can be set via a signal-slot while the loop executes
        signalNeverBad = signalNeverBad && flagSignalFine;
        if(params->isOnline){
            emit onlinePeriodStarted();
        }
        // reset the early classification flag
        flagPeriodClassifiedEarly = false;
        for(int flashNo=0; flashNo<params->epochsPerStimulus && signalNeverBad && !flagAbort
            && !flagPeriodClassifiedEarly; ++flashNo){
            // a bit redundant variable it seems
            online_epochsElapsed = flashNo+1;
            // qDebug()<<"Flashing everything: "<<flashNo<<"/"<<epochsPerStimulus;
            QVector<short>* stimuli = blockRandomizeFlashes();
            for(int blockNo = 0; blockNo<stimuli->length() && signalNeverBad && !(flagAbort|flagPeriodClassifiedEarly); ++blockNo){
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
            // too late, now the EndOfPeriod classification will take place
            if( params->isOnline && flashNo<params->epochsPerStimulus-1 ){
                emit signalOnlineEpochEnded();
            }
        }
        // qDebug()<<QString("inter-period sleep for target %1 of %2 ...").arg(targetIdx+1).arg(phrase.size());
        emit commandDimKeyboard();
        //thread->msleep(interPeriodInterval);
        this->sleepFrameAligned(params->stintInterPeriod);
        if(params->isOnline){
            emit onlinePeriodEnded();
        }
    }
    // qDebug()<<QString("### DONE in thread: %1 ###").arg(QThread::currentThread()->objectName());
    if(!signalNeverBad){
        emit error(ERRCODE_SIGNAL);
    }else if(flagAbort){
        emit error(ERRCODE_ABORTED);
    }

    if(params->isOnline){
        emit onlineModeEnded();
    }else{
        // Make sure the dumper doesn't capture more frames
        emit dataTakingEnded();
    }
    disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), dumper, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    delete params;
}

void SpellerController::startDataTaking(QString phrase, int epochsPerStimulus, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,
                                        QString subjectName, QString parentDirectory){
    dataTakingParams* params = new dataTakingParams();

    params->isOnline=false;
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

void SpellerController::startOnline(int charactersLimit, int epochsPerStimulus, int interStimulusInterval,
                                    int interPeriodInterval, int highlightDuration, int infoDuration){

    dataTakingParams* params = new dataTakingParams();

    params->isOnline=true;
    params->symbolsLimit=qMax(0, charactersLimit);
    params->stintHighlight=highlightDuration;
    params->stintDim=interStimulusInterval-highlightDuration;
    params->stintInfo=infoDuration;
    params->stintInterPeriod=interPeriodInterval;
    params->epochsPerStimulus=epochsPerStimulus;

    QtConcurrent::run(this, &SpellerController::dataTakingJob, params);
}

void SpellerController::endOnline(){
    flagAbort = true;
}

void SpellerController::slotSignalFine(bool isIt){
    this->flagSignalFine = isIt || !Settings::qcInterrupt();
}

void SpellerController::slotCapturedOnlinePeriod(QSharedPointer<QVector<int> > data, QSharedPointer<QVector<int> > meta, QSharedPointer<QVector<int> > trg){
    qDebug()<<"Got some data captured online: "<<data->length()<<" samples of data, "<<meta->length()<<" samples of meta and "<<trg->length()<<" samples of trg";
    emit requestPeriodClassification(data, meta, trg);
}

void SpellerController::slotCapturedOnlineEpoch(QSharedPointer<QVector<int> > data, QSharedPointer<QVector<int> > meta, QSharedPointer<QVector<int> > trg){
    qDebug()<<"Got an epoch captured online: "<<data->length()<<" samples of data, "<<meta->length()<<" samples of meta and "<<trg->length()<<" samples of trg";
    emit requestPeriodClassification(data, meta, trg);
}

/**
    simplified: print character only if confidence threshold was met
**/
void SpellerController::slotSymbolRecognized(int row, int col, float confidence){
    qDebug()<<"SpellerController::slotSymbolRecognized " << confidence;
    if(this->online_epochsElapsed > Settings::getOnlineMinEpochs() &&
            confidence>Settings::getConfidenceThreshold() && !flagPeriodClassifiedEarly){
        //TODO might need synchro
        flagPeriodClassifiedEarly = true;
        int symbolNo = this->symbolRowColToNumber(row, col);
        QString symbolStr = this->keyboardSymbols.at(symbolNo);
        if(symbolStr.length()==1){
            emit signalSymbolRecognized(symbolStr.at(0));
        }else{
            emit signalSymbolRecognized(symbolStr);
        }
    }
}

void SpellerController::slotNewDaq(const EegDaq *newDaq){
    qDebug()<<"Speller controller  notified of the new DAQ";
    this->daq = newDaq;
}
