#include "tst_spellerreceivermock.h"
#include <QDebug>
#include <QThread>

SpellerReceiverMock::SpellerReceiverMock(QObject *parent) :
    QObject(parent)
{
}


void SpellerReceiverMock::highlightRowCol(short code){
    qDebug()<<"to be implemented";
}
void SpellerReceiverMock::displayInformation(QString msg){
    qDebug()<<"to be implemented";
}
void SpellerReceiverMock::indicateTarget(short row, short column){
    qDebug()<<"to be implemented";
}
void SpellerReceiverMock::dim(){
    qDebug()<<"to be implemented";
}
void SpellerReceiverMock::dataTakingEnded(){
    qDebug()<<"ReceiverMock got the dataTakingEnded signal. Terminating receiver thread: "<<QThread::currentThread()->objectName();
}
