#include "fakedaq.h"
#include <QtGlobal>
#include <QDebug>
#include <QSharedPointer>
#include <qmath.h>
#include <timer.h>
#include <iostream>

FakeDaq::FakeDaq()
{
}

void FakeDaq::run(){
    while(!_shutdown){
        msleep((long)(1000.0/128));

        double time = Timer::getTime()/1000.0;
        int noise = 50 -(qrand()%100);

        QSharedPointer<EegFrame> eegFrame = QSharedPointer<EegFrame>(new EegFrame(
                                                                         (int)(5000+5000*qSin(time)),            (int)(5000+5000*qCos(time)),
                                                                         (int)(5000+5000*qSin(time+noise)),      (int)(5000+5000*qCos(time+noise)),
                                                                         (int)(5000+5000*qSin(2*time)),          (int)(5000+5000*qCos(2*time)),
                                                                         (int)(5000+5000*qSin(2*time+noise)),    (int)(5000+5000*qCos(2*time+noise)),
                                                                         (int)(5000+5000*qSin(4*time)),          (int)(5000+5000*qCos(4*time)),
                                                                         (int)(5000+5000*qSin(4*time+noise)),    (int)(5000+5000*qCos(4*time+noise)),
                                                                         (int)(5000+5000*qSin(8*time)),          (int)(5000+5000*qCos(8*time))));

        time=time/10;


        if(qrand()%1000>950){
            QSharedPointer<MetaFrame> metaFrame = QSharedPointer<MetaFrame>(new MetaFrame(
                                                                                (unsigned char)0,(char)0,(char)0,(unsigned char)(100.0 - time),
                                                                                qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),
                                                                                qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),
                                                                                qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),
                                                                                qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0)),
                                                                                qSin(time*(1+(qrand()%100)/1000.0)),qSin(time*(1+(qrand()%100)/1000.0))));

            //                (qrand()%1000)/1000.0,(qrand()%1000)/1000.0,(qrand()%1000)/1000.0,(qrand()%1000)/1000.0,
            //                (qrand()%1000)/1000.0,(qrand()%1000)/1000.0,(qrand()%1000)/1000.0,(qrand()%1000)/1000.0,
            //                (qrand()%1000)/1000.0,(qrand()%1000)/1000.0,(qrand()%1000)/1000.0,(qrand()%1000)/1000.0,
            //                (qrand()%1000)/1000.0,(qrand()%1000)/1000.0));

            this->metaFrame(metaFrame);
        }
        this->eegFrame(eegFrame);
    }
}
