#include "metaprocessor.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

MetaProcessor::MetaProcessor() :
    QObject()
{
    signalsFineSoFar=false;
}

bool MetaProcessor::signalFine() const{
    return signalsFineSoFar;
}

void MetaProcessor::metaFrame(QSharedPointer<MetaFrame> eegFrame){
    processMetaFrame(eegFrame);
}

void MetaProcessor::processMetaFrame(QSharedPointer<MetaFrame> framePtr){
    bool signalsFineNow=true;
    for(unsigned char i=0; i<EegFrame::CONTACTS_NO; ++i){
        if(framePtr->getQuality(i)<=0.8){
            signalsFineNow=false;
            break;
        }
    }
    if(signalsFineNow!=this->signalsFineSoFar){
        signalsFineSoFar = signalsFineNow;
        emit signalFine(signalsFineNow);
    }
}
