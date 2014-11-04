#include "spellercontroller.h"
#include <cstdlib>
#include <QStringList>
#include <QDebug>
#include <QTime>
#include <QMutex>

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
    thread->setObjectName("SpellerControllerThread");

    thread->start();

    /** set the Dumper up **/
    //TODO: remove hardcoded path
    dumper = new SpellerDumper();
    dumperThread = new QThread();
    dumper->moveToThread(dumperThread);
    dumperThread->start();

    connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), dumper, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    connect(this, SIGNAL(commandRowColHighlight(short)), dumper, SLOT(spellerHighlight(short)));
    connect(this, SIGNAL(dataTakingStarted(QString, QString)), dumper, SLOT(startDumpingSession(QString, QString)));
    connect(this, SIGNAL(dataTakingEnded()), dumper, SLOT(closeDumpingSession()));

}

SpellerController::~SpellerController(){
    //should it be auto-deleted through parentage??
    delete thread;
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
    unsigned long startTime = QTime::currentTime().msec();
    QMutex mutex;
    while(QTime::currentTime().msec()-startTime<duration){
        mutex.lock();
        daq->getFrameEmittedWaitCondition()->wait(&mutex);
        qDebug()<<"frame aligned sleep slept for "<<(QTime::currentTime().msec()-startTime)<<" of ouf "<<duration;
        mutex.unlock();
    }
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


void SpellerController::startDataTaking(QString phrase, int epochsPerStimulus, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration){
    // qDebug("Controller received a startDataTaking signal");

    if(!validateInputString(phrase) || !validateTimings(interStimulusInterval, interPeriodInterval, highlightDuration, infoDuration)){
        //TODO: should emit data taking over with an error code
        return;
    }else{
        emit dataTakingStarted("DefaultSubject", "/tmp/eeg.dumps/");
    }

    for(int targetIdx = 0; targetIdx<phrase.length(); ++targetIdx){
        QString target = phrase.mid(targetIdx, 1);
        unsigned short int number = keyboardSymbols.indexOf(target);
        unsigned short int row;
        unsigned short int column;
        symbolNumberToRowCol(number, row, column);

        emit commandNextPeriod();
        //qDebug()<<"Instructing to focus on column "<<column<<", row "<<row;
        emit commandIndicateTarget(row, column);

        //thread->msleep(infoDuration);
        this->sleepFrameAligned(infoDuration);


        // qDebug()<<"Dimming for short";
        emit commandDimKeyboard();
        //thread->msleep(interStimulusInterval);
        this->sleepFrameAligned(interStimulusInterval);

        for(int flashNo=0; flashNo<epochsPerStimulus; ++flashNo){
            // qDebug()<<"Flashing everything: "<<flashNo<<"/"<<epochsPerStimulus;
            QVector<short>* stimuli = blockRandomizeFlashes();
            for(int blockNo = 0; blockNo<stimuli->length(); ++blockNo){
                //  qDebug()<<"Flashing stimulus of code: "<<stimuli->at(blockNo);
                emit commandRowColHighlight(stimuli->at(blockNo));
                // thread->msleep(highlightDuration);
                this->sleepFrameAligned(highlightDuration);
                //  qDebug()<<"Dimming for short";
                emit commandDimKeyboard();
                //thread->msleep(interStimulusInterval-highlightDuration);
                this->sleepFrameAligned(interStimulusInterval-highlightDuration);
            }
            delete stimuli;
        }

        // qDebug()<<QString("inter-period sleep for target %1 of %2 ...").arg(targetIdx+1).arg(phrase.size());
        emit commandDimKeyboard();
        //thread->msleep(interPeriodInterval);
        this->sleepFrameAligned(interPeriodInterval);
    }
    // qDebug()<<QString("### DONE in thread: %1 ###").arg(QThread::currentThread()->objectName());
    emit dataTakingEnded();
}
void SpellerController::endDataTaking(){}

void SpellerController::startOnline(int epochsPerStimulus, int interStimulusInterval, int highlightDuration, int infoDuration){}
void SpellerController::endOnline(){}


