#include "spellercontroller.h"
#include <cstdlib>

SpellerController::SpellerController(unsigned short int newMatrixSize, QObject *parent) :
    QObject(parent), matrixSize(newMatrixSize)
{
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


void SpellerController::startDataTaking(QString phrase, int epochsPerStimuli, int interStimulusInterval, int highlightDuration, int infoDuration){}
void SpellerController::endDataTaking(){}

void SpellerController::startOnline(int epochsPerStimuli, int interStimulusInterval, int highlightDuration, int infoDuration){}
void SpellerController::endOnline(){}
