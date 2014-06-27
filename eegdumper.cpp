#include "eegdumper.h"
#include "QDebug"

EegDumper::EegDumper() :
    EegProcessor(), file("/home/bawey/Desktop/testOutput"), out(&file)
{
    file.open(QIODevice::WriteOnly | QIODevice::Text);
}

void EegDumper::eegFrame(QSharedPointer<EegFrame> eegFrame){
   out<<*eegFrame;
   out.flush();
}

EegDumper::~EegDumper(){
    qDebug()<<"Closing the output file. yeah. hell yes!";
    file.close();
}
