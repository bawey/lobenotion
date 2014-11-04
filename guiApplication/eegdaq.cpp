#include "eegdaq.h"

EegDaq::EegDaq() :
    QThread()
{
    frameEmitted=new QWaitCondition();
    qRegisterMetaType<QSharedPointer<EegFrame>>("QSharedPointer<EegFrame>");
    qRegisterMetaType<QSharedPointer<EegFrame>>("QSharedPointer<EegFrame>");

    connect(this, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), this, SLOT(frameProducedSlot()));
    this->moveToThread(this);
}

void EegDaq::shutdown(){
    _shutdown=true;
}

QWaitCondition* const EegDaq::getFrameEmittedWaitCondition() const{
    return frameEmitted;
}
