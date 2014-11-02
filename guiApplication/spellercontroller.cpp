#include "spellercontroller.h"
#include <cstdlib>
#include <QStringList>

SpellerController::SpellerController(unsigned short int newMatrixSize, QString symbols, QObject *parent) :
    QObject(parent), matrixSize(newMatrixSize)
{
    this->keyboardSymbols=QVector<QString>();

    QStringList tokens = symbols.split(' ');

    for(int i=0; i<tokens.length(); ++i){
        this->keyboardSymbols.push_back(tokens.at(i));
    }

    thread = new QThread(this);
    this->moveToThread(thread);
}

SpellerController::~SpellerController(){
    thread->terminate();
    //should be auto-deleted through parentage
    //delete thread;
}

QVector<short int>* SpellerController::blockRandomizeFlashes(){
    QVector<short int>* stimuli = new QVector<short int>();
    QVector<short int>* rowStimuli = new QVector<short int>();
    QVector<short int>* colStimuli = new QVector<short int>();

    rowStimuli->push_back(1);
    colStimuli->push_back(-1);
    for(int i=1; i<this->matrixSize; ++i){
        rowStimuli->insert((int)((rowStimuli->size()-1)*((float)qrand()/RAND_MAX)), i+1);
        colStimuli->insert((int)((colStimuli->size()-1)*((float)qrand()/RAND_MAX)), -(i+1));
    }

    for(int i=0; i<matrixSize; ++i){
        stimuli->push_back(rowStimuli->at(i));
        stimuli->push_back(colStimuli->at(i));
    }

    delete rowStimuli;
    delete colStimuli;

    return stimuli;
}


void SpellerController::startDataTaking(QString phrase, int epochsPerStimulus, int interStimulusInterval, int highlightDuration, int infoDuration){
    if(!validateInputString(phrase) || !validateTimings(interStimulusInterval, interPeriodInterval, highlightDuration, infoDuration)){
        //should emit data taking over with an error code
        return;
    }

    this->epochsPerStimulus=epochsPerStimulus;
    this->interStimulusInterval=interStimulusInterval;
    this->highlightDuration = highlightDuration;
    this->infoDuration = infoDuration;

    for(int targetIdx = 0; targetIdx<phrase.length(); ++targetIdx){
        QString target = phrase.mid(targetIdx, 1);
        unsigned short int number = keyboardSymbols.indexOf(target);
        unsigned short int row;
        unsigned short int column;
        symbolNumberToRowCol(number, row, column);

        thread->sleep(200);

        //send the instruction: please focus on this and that letter
    }

}
void SpellerController::endDataTaking(){}

void SpellerController::startOnline(int epochsPerStimulus, int interStimulusInterval, int highlightDuration, int infoDuration){}
void SpellerController::endOnline(){}
