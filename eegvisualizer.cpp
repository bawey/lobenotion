#include "eegvisualizer.h"
#include <QDebug>
#include <timer.h>
#include <QList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

//static int EegVisualizer::EEG_BUFFER_LENGTH;
//static int EegVisualizer::META_BUFFER_LENGTH;

//QCustomPlot EegVisualizer::plot;

EegVisualizer::EegVisualizer()
{
    window.resize(640, 480);
    window.show();
    window.setWindowTitle("PlotSS!");

    QVBoxLayout* column = new QVBoxLayout();

    for(int i=0; i<EegFrame::CONTACTS_NO; ++i){
        QHBoxLayout *row = new QHBoxLayout();
        {
            QLabel *label = new QLabel(QString(EegFrame::contacts[i].c_str())+
                                       QString(EegFrame::contacts[i+1].c_str()));
            QCustomPlot *plot = new QCustomPlot();
            plot->xAxis->setLabel("t");
            plot->yAxis->setLabel("v");
            plot->yAxis->setRange(8000, 10000);
            graphs[i]=plot->addGraph();
            graphs[++i]=plot->addGraph();
            plot->graph(1)->setPen(QPen(QColor(255,0,0,255)));
            row->addWidget(label);
            row->addWidget(plot);
        }
        column->addLayout(row);
    }

    window.setLayout(column);
}

void EegVisualizer::processEegFrame(QSharedPointer<EegFrame> framePtr){
    static QList<double> x[EegFrame::CONTACTS_NO];
    static QList<double> y[EegFrame::CONTACTS_NO];
    static unsigned short counter = 0;

    counter=(counter+1)%32;

    for(int e=0; e<EegFrame::CONTACTS_NO; ++e){

        x[e].push_back((double)framePtr->timestamp);
        y[e].push_back((double)framePtr->getValue(e));


        if(counter==31){
            this->graphs[e]->setData(x[e].toVector(), y[e].toVector());

            // set axes ranges, so we see all data:
            graphs[e]->parentPlot()->xAxis->setRange(x[e].at(0), x[e].at(x[e].size()-1));

            graphs[e]->parentPlot()->replot();

        }
        if(x[e].size()>=2000){
            x[e].pop_front();
            y[e].pop_front();
        }
    }
}

void EegVisualizer::run(){

    int nextToRead=0;
    while(true){
//        qDebug()<<"consumer looking for lock";
        lock.lock();
//        qDebug()<<"consumer got the lock";
        if(eegBufferedCount==0){
//            qDebug()<<"Consumer waiting for data";
            bufferNotEmpty.wait(&lock);
        }
        //else{
        lock.unlock();
        //}
//        qDebug()<<"consumer released the lock";

        /** consume **/
        QSharedPointer<EegFrame> frame = eegBuffer[nextToRead];
        processEegFrame(frame);
//        std::cout<<"Read from buffer: "<<*frame<<std::endl;
//        std::cout<<"Buffer usage: "<<eegBufferedCount<<std::endl;
        nextToRead = (nextToRead+1)%EEG_BUFFER_LENGTH;

//        qDebug()<<"consumer looking for lock";
        lock.lock();
        if(eegBufferedCount--==EEG_BUFFER_LENGTH){
            bufferNotFull.wakeAll();
        }
        lock.unlock();
//        qDebug()<<"consumer released the lock";

    }
}

void EegVisualizer::eegFrame(QSharedPointer<EegFrame> eegFrame){
    static int nextToWrite=0;
//    qDebug()<<"producer delivers a frame - will acquire lock";
    lock.lock();
//    qDebug()<<"producer acquired lock";
    if(eegBufferedCount==EEG_BUFFER_LENGTH){
//        qDebug()<<"producer waits for buffer space";
        bufferNotFull.wait(&lock);
    }
    //else{
    lock.unlock();
    //}
//    qDebug()<<"Producer released lock";

    /** produce **/
    eegBuffer[nextToWrite]=eegFrame;
    nextToWrite=(nextToWrite+1)%EEG_BUFFER_LENGTH;
//    qDebug()<<"producer put sth in buffer";

//    qDebug()<<"producer looking for lock";
    lock.lock();
//    qDebug()<<"producer has the lock";
    if(eegBufferedCount++==0){
//        qDebug()<<"producer wakes consumers";
        bufferNotEmpty.wakeAll();
    }
    lock.unlock();
//    qDebug()<<"consumer released the lock";
}
