#include "fakedaq.h"
#include <QtGlobal>
#include <QDebug>
#include <QSharedPointer>
#include <qmath.h>
#include <timer.h>
#include <iostream>
#include <settings.h>

FakeDaq::FakeDaq()
{
}

float FakeDaq::generateContactQuality(float time){
    if(Settings::isDummyDaqNoisy()){
        return qAbs(qSin(time*(1+(qrand()%100)/1000.0)));
    }else{
        return 0.85f;
    }
}

void FakeDaq::run(){
    while(!_shutdown){
        msleep((long)(1000.0/128));

        double time = Timer::getTime()/1000.0;
        int noise = 50 -(qrand()%100);

        QSharedPointer<EegFrame> eegFrame = QSharedPointer<EegFrame>(new EegFrame(
                                                                         (int)(9000+1000*qSin(.1*time+.01*noise)),            (int)(9000+1000*qCos(.1*time+.01*noise)),
                                                                         (int)(9000+1000*qSin(.1*(time+.01*noise))),      (int)(9000+1000*qCos(.1*(time+.01*noise))),
                                                                         (int)(9000+1000*qSin(.2*time+.01*noise)),          (int)(9000+1000*qCos(.2*time+.01*noise)),
                                                                         (int)(9000+1000*qSin(.2*(time+.01*noise))),    (int)(9000+1000*qCos(.2*(time+.01*noise))),
                                                                         (int)(9000+1000*qSin(.4*time+.01*noise)),          (int)(9000+1000*qCos(.4*time+.01*noise)),
                                                                         (int)(9000+1000*qSin(.4*(time+.01*noise))),    (int)(9000+1000*qCos(.4*(time+.01*noise))),
                                                                         (int)(9000+1000*qSin(.8*time+.01*noise)),          (int)(9000+1000*qCos(.8*time+.01*noise))));

        time=time/10;


        if(qrand()%1000>0){
            QSharedPointer<MetaFrame> metaFrame = QSharedPointer<MetaFrame>(new MetaFrame(
                                                                                (unsigned char)0,(char)0,(char)0,(unsigned char)(100.0 - time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time),
                                                                                generateContactQuality(time)));

            emit this->metaFrame(metaFrame);
        }
        emit this->eegFrame(eegFrame);
    }
}
