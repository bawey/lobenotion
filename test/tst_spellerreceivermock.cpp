#include "tst_spellerreceivermock.h"
#include <QDebug>
#include <QThread>

SpellerReceiverMock::SpellerReceiverMock(QObject *parent) :
    QObject(parent)
{
}

SpellerReceiverMock::~SpellerReceiverMock(){
    qDebug()<<"SpellerReceiverMock destructor invoked in thread: "<<QThread::currentThread()->objectName();
    delete targets;
    delete stimuliInPeriods;
    delete dimsInPeriods;
}

void SpellerReceiverMock::highlightRowCol(short code){
    QMap<short, unsigned short>* map = stimuliInPeriods->at(periodsCount-1);
    if(!map->contains(code)){
        map->insert(code, 1);
    }else{
        map->insert(code, map->value(code)+1);
    }
}
void SpellerReceiverMock::displayInformation(QString msg){
    qDebug()<<"Some information was meant to be displayed now: "<<msg;
}
void SpellerReceiverMock::indicateTarget(short row, short column){
    targets->at(periodsCount-1)->first=row;
    targets->at(periodsCount-1)->second=column;
}
void SpellerReceiverMock::dim(){
    dimsInPeriods->replace(periodsCount-1, dimsInPeriods->at(periodsCount-1)+1);
}
void SpellerReceiverMock::dataTakingEnded(){
    notifiedAboutSessionEnd=true;
    sessionEndReceivedByThreadName=QThread::currentThread()->objectName();
    //qDebug()<<"ReceiverMock got the dataTakingEnded signal in the thread: "<<sessionEndReceivedByThreadName;
}
void SpellerReceiverMock::nextPeriod(){
    ++periodsCount;

    targets->append(new QPair<unsigned short, unsigned short>((unsigned short)0,(unsigned short)0));
    stimuliInPeriods->append(new QMap<short, unsigned short>());
    dimsInPeriods->append(0);

//    qDebug("ReceiverMock notified about next period");
}
