#include "epocdaq.h"


EpocDaq::EpocDaq()
{
    epocDev = emokit_create();
    //epoc daemon - need that variable?
    this->isDaemonRunning=true;
}

EpocDaq::~EpocDaq(){
    emokit_close(this->epocDev);
    emokit_delete(this->epocDev);
}

void EpocDaq::run(){
    int response = emokit_open(epocDev, EMOKIT_VID, EMOKIT_PID, 1);
    if(response != 0)
    {
        qDebug("CANNOT CONNECT: %d\n", response);
    }

    while(true){
        if(!this->isDaemonRunning){
            this->sleep(500);
            continue;
        }

        struct emokit_frame c;
        if(emokit_read_data(epocDev) > 0)
        {
            c = emokit_get_next_frame(epocDev);
            QSharedPointer<EegFrame> eegFramePtr   = QSharedPointer<EegFrame>(
                        new EegFrame(c.AF3, c.AF4, c.F3, c.F4, c.F7, c.F8, c.FC5,
                                     c.FC6, c.T7, c.T8, c.P7, c.P8, c.O1, c.O2));
            QSharedPointer<MetaFrame> metaFramePtr = QSharedPointer<MetaFrame>(
                        new MetaFrame(c.counter, c.gyroX, c.gyroY, c.battery,
                                      c.cq.AF3/540.0f, c.cq.AF4/540.0f, c.cq.F3/540.0f, c.cq.F4/540.0f,
                                      c.cq.F7/540.0f, c.cq.F8/540.0f, c.cq.FC5/540.0f, c.cq.FC6/540.0f,
                                      c.cq.T7/540.0f, c.cq.T8/540.0f, c.cq.P7/540.0f, c.cq.P8/540.0f,
                                      c.cq.O1/540.0f, c.cq.O2/540.0f)
                        );

            emit eegFrame(eegFramePtr);
            emit metaFrame(metaFramePtr);

        }else{
            qDebug("emokit_read_data returned 0. What does it mean?");
        }
    }
}

