#include "eegdaq.h"
#include <QDebug>

EegDaq::EegDaq() :
    QObject()
{
    frameEmitted=new QWaitCondition();
    qRegisterMetaType<QSharedPointer<EegFrame>>("QSharedPointer<EegFrame>");
    qRegisterMetaType<QSharedPointer<MetaFrame>>("QSharedPointer<MetaFrame>");

    connect(this, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), this, SLOT(frameProducedSlot()));
}

void EegDaq::shutdown(){
    _shutdown=true;
}

QWaitCondition *EegDaq::getFrameEmittedWaitCondition() const{
    return frameEmitted;
}

void EegDaq::slotLaunch(){
    qDebug()<<"Daq slotLaunch at work";
    run();
}
