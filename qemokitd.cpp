#include "qemokitd.h"
#include <QtDebug>
#include "emokit/emokit.h"
#include <QString>
#include <QTime>

QEmokitD::QEmokitD()
{
    d = emokit_create();
    isDaemonRunning = true;
}

QEmokitD::~QEmokitD(){
    emokit_close(d);
    emokit_delete(d);
}

void QEmokitD::run(){
    int r = emokit_open(d, EMOKIT_VID, EMOKIT_PID, 1);
    if(r != 0)
        if(r != 0)
    {
        qDebug("CANNOT CONNECT: %d\n", r);
    }

    int devCount = emokit_get_count(d, EMOKIT_VID, EMOKIT_PID);
    setDevicesCount(devCount);

    int prevCount = -1;

    QTime timer;
    timer.start();
    long frames;
    qDebug()<<"Entering daemon's main loop. June";
    while(true){
        if(!this->isDaemonRunning){
            qDebug()<<"Sleeping casue daemon ain't running";
            this->sleep(500);
            /** monitor for changes of devices count **/
            qDebug()<<"Waking up";
            continue;
        }
        if(devCount!=this->devicesCount){
            setDevicesCount(devCount);
        }

        struct emokit_frame c;
        qDebug()<<"Gonna read...";
        if(emokit_read_data(d) > 0)
        {
            qDebug()<<"Read sth";

            ++frames;
            c = emokit_get_next_frame(d);
            fprintf(stdout,"\033[H\033[2JPress CTRL+C to exit\n\nContact quality:\nF3  %4d\nFC6 %4d\nP7  %4d\nT8  %4d\nF7  %4d\nF8  %4d\nT7  %4d\nP8  %4d\nAF4 %4d\nF4  %4d\nAF3 %4d\nO2  %4d\nO1  %4d\nFC5 %4d"
                    "\n\nContact measures:\nF3  %4d\nFC6 %4d\nP7  %4d\nT8  %4d\nF7  %4d\nF8  %4d\nT7  %4d\nP8  %4d\nAF4 %4d\nF4  %4d\nAF3 %4d\nO2  %4d\nO1  %4d\nFC5 %4d"
                    ,c.cq.F3, c.cq.FC6, c.cq.P7, c.cq.T8,c.cq.F7, c.cq.F8, c.cq.T7, c.cq.P8, c.cq.AF4, c.cq.F4, c.cq.AF3, c.cq.O2, c.cq.O1, c.cq.FC5
                    ,c.F3, c.FC6, c.P7, c.T8,c.F7, c.F8, c.T7, c.P8, c.AF4, c.F4, c.AF3, c.O2, c.O1, c.FC5);
            fflush(stdout);
            if(prevCount!=-1){
                if(prevCount==c.counter){
                    qDebug()<<"Packet numbers repeated";
                }else if(((prevCount+1) % 129)!=c.counter){
                    qDebug()<<QString("skipped frame between %1 and %2").arg(prevCount).arg(c.counter);

                }}
            prevCount=c.counter;
            qDebug()<<QString::number(c.counter);

        }else{
            qDebug("emokit_read_data returned 0. What does it mean?");
        }

        if(timer.elapsed()>333){
            double rate = frames*1000/(double)timer.elapsed();
            qDebug() <<QString("got %1 frames within %2 seconds. avg frequency: %3 Hz").arg(frames).arg(timer.elapsed()/1000.0).arg(rate);
            emit dataRate(rate);
            frames = 0;
            timer.restart();
        }
    }
}

void QEmokitD::setRunning(bool onOff){
    this->isDaemonRunning=onOff;
    if (isDaemonRunning){
        emit resumed();
    } else {
        emit paused();
    }
}


void QEmokitD::setDevicesCount(int newCount){
    if(this->devicesCount!=newCount){
        emit devicesCountChanged(newCount);
    }
    this->devicesCount=newCount;
}
