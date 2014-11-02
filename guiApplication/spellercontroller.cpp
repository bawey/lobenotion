#include "spellercontroller.h"
#include <cstdlib>
#include <QStringList>
#include <QDebug>

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
    thread->setObjectName("SpellerControllerThread");

    thread->start();

}

SpellerController::~SpellerController(){
    //should it be auto-deleted through parentage??
    delete thread;
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
    qDebug("Controller received a startDataTaking signal");

    if(!validateInputString(phrase) || !validateTimings(interStimulusInterval, interPeriodInterval, highlightDuration, infoDuration)){
        //should emit data taking over with an error code
        return;
    }

//    this->epochsPerStimulus=epochsPerStimulus;
//    this->interStimulusInterval=interStimulusInterval;
//    this->highlightDuration = highlightDuration;
//    this->infoDuration = infoDuration;

    for(int targetIdx = 0; targetIdx<phrase.length(); ++targetIdx){
        QString target = phrase.mid(targetIdx, 1);
        unsigned short int number = keyboardSymbols.indexOf(target);
        unsigned short int row;
        unsigned short int column;
        symbolNumberToRowCol(number, row, column);

        qDebug()<<"Instructing to focus on column "<<column<<", row "<<row;
        thread->msleep(infoDuration);

        qDebug()<<"Dimming for short";
        thread->msleep(interStimulusInterval);

        for(int flashNo=0; flashNo<epochsPerStimulus; ++flashNo){
            qDebug()<<"Flashing everything: "<<flashNo<<"/"<<epochsPerStimulus;
            QVector<short>* stimuli = blockRandomizeFlashes();
            for(int blockNo = 0; blockNo<stimuli->length(); ++blockNo){
//                qDebug()<<"Flashing stimulus of code: "<<stimuli->at(blockNo);
                thread->msleep(highlightDuration);
//                qDebug()<<"Dimming for short";
                thread->msleep(interStimulusInterval-highlightDuration);
            }
            delete stimuli;
        }

        qDebug()<<QString("inter-period sleep for target %1 of %2 ...").arg(targetIdx+1).arg(phrase.size());
        thread->msleep(interPeriodInterval);

        //send the instruction: please focus on this and that letter
    }
    qDebug()<<QString("### DONE in thread: %1 ###").arg(QThread::currentThread()->objectName());
    emit dataTakingEnded();
}
void SpellerController::endDataTaking(){}

void SpellerController::startOnline(int epochsPerStimulus, int interStimulusInterval, int highlightDuration, int infoDuration){}
void SpellerController::endOnline(){}
