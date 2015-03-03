#include "metaprocessor.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <settings.h>

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
    int poorChannels = 0;
    for(unsigned char i=0; i<EegFrame::CONTACTS_NO; ++i){
        if(framePtr->getQuality(i)<Settings::qcGoodnessLevel()){
            ++poorChannels;
        }
    }
    signalsFineNow = poorChannels <= Settings::qcChannelsTolerance();
    if(!broadcasted || signalsFineNow!=this->signalsFineSoFar){
        broadcasted = true;
        signalsFineSoFar = signalsFineNow;
        emit signalFine(signalsFineNow);
    }
}

void MetaProcessor::configurationChanged(){
//    qDebug()<<"broadcastong settings-update forced signal quality";
    emit signalFine(signalsFineSoFar);
}
