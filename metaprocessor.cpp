#include "metaprocessor.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>

MetaProcessor::MetaProcessor() :
    QThread()
{
    QVBoxLayout* column = new QVBoxLayout();
    for(int i=0; i<EegFrame::CONTACTS_NO; ++i){
        QHBoxLayout* row = new QHBoxLayout();
        QLabel* label = new QLabel(EegFrame::contacts[i].c_str());
        QLabel* value = new QLabel("---");
        row->addWidget(label);
        row->addWidget(value);
        qualityLabels[i]=value;
        column->addLayout(row);
    }

    QHBoxLayout* signalRow = new QHBoxLayout();
    signalRow->addWidget(new QLabel("Signal:"));
    signalRow->addWidget(&signalQualityLabel);
    signalQualityLabel.setText("Initializing...");
    column->addLayout(signalRow);

    QHBoxLayout* batteryRow = new QHBoxLayout();
    batteryRow->addWidget(new QLabel("Battery: "));
    batteryRow->addWidget(&batteryLevelWidget);
    batteryLevelWidget.setText("Initializing...");
    column->addLayout(batteryRow);

    QHBoxLayout* bufferRow = new QHBoxLayout();
    bufferRow->addWidget(new QLabel("Buffer:"));
    bufferRow->addWidget(&metaBufferWidget);
    column->addLayout(bufferRow);

    widget.setLayout(column);
    widget.show();

    signalsFineSoFar=false;
}

void MetaProcessor::run(){
    int nextToRead=0;
    while(true){
        lock.lock();
        if(metaBufferedCount==0){
            bufferNotEmpty.wait(&lock);
        }
        lock.unlock();

        /** consume **/
        QSharedPointer<MetaFrame> frame = metaBuffer[nextToRead];
        processMetaFrame(frame);
        std::cout<<"Buffer usage: "<<metaBufferedCount<<std::endl;
        nextToRead = (nextToRead+1)%META_BUFFER_LENGTH;

        lock.lock();
        if(metaBufferedCount--==META_BUFFER_LENGTH){
            bufferNotFull.wakeAll();
        }
        lock.unlock();
    }
}


void MetaProcessor::metaFrame(QSharedPointer<MetaFrame> metaFrame){
    static int nextToWrite=0;
    lock.lock();
    if(metaBufferedCount==META_BUFFER_LENGTH){
        bufferNotFull.wait(&lock);
    }
    lock.unlock();

    /** produce **/
    metaBuffer[nextToWrite]=metaFrame;
    nextToWrite=(nextToWrite+1)%META_BUFFER_LENGTH;
    lock.lock();
    if(metaBufferedCount++==0){
        bufferNotEmpty.wakeAll();
    }
    lock.unlock();
}

void MetaProcessor::processMetaFrame(QSharedPointer<MetaFrame> framePtr){
    bool signalsFineNow=true;
    for(int i=0; i<EegFrame::CONTACTS_NO; ++i){
       if(framePtr->getQuality(i)<=0.8){
           signalsFineNow=false;
           //qualityLabels[i]->setStyleSheet("QLabel { color : red; }");
       }else{
           //qualityLabels[i]->setStyleSheet("QLabel { color : green; }");
       }

       qualityLabels[i]->setText(QString::number(framePtr->getQuality(i),'f',3));
       if(qrand()%100>90){
           //widget.repaint();
       }
    }
    // good goes bad or vice versa
    if(this->signalsFineSoFar!=signalsFineNow){
        emit signalFine(signalsFineNow);
        this->signalsFineSoFar=signalsFineNow;
    }
    if(signalsFineSoFar){
        signalQualityLabel.setText("fine");
        signalQualityLabel.setStyleSheet("QLabel { color : green; }");
    }else{
        signalQualityLabel.setText("noisy");
        signalQualityLabel.setStyleSheet("QLabel { color : red; }");
    }

    batteryLevelWidget.setText(QString::number(framePtr->battery));
    metaBufferWidget.setText(QString::number(metaBufferedCount)+QString("/")
                             +QString::number(META_BUFFER_LENGTH));

}
