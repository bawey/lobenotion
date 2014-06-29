#include "eegdumper.h"
#include "QDebug"
#include "QString"
#include "settings.h"

EegDumper::EegDumper() :
    EegProcessor()
{

}

void EegDumper::eegFrame(QSharedPointer<EegFrame> eegFrame){
    if(out==0){
        qWarning()<<"EEG frame dumping attempted after file closing.";
    }else{
        *out<<*eegFrame;
        out->flush();
    }
}

void EegDumper::closeDumpingSession(){
    out->flush();
    file->close();
    delete out;
    delete file;
    out=0;
    file=0;
    qDebug()<<"EEG dumper closed";
}

void EegDumper::startDumpingSession(){
    unsigned int suffix=1;

    QString path = Settings::getEegDumpPath();

    while(QFile::exists(path)){
        path=Settings::getEegDumpPath()+QString::number(suffix++);
    }

    file = new QFile(path);
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    out= new QTextStream(file);
    qDebug()<<"EEG dumper started";
}

EegDumper::~EegDumper(){

}

