#include "spellerdumper.h"
#include <timer.h>
#include <settings.h>
#include <QString>
#include <QDebug>
#include <QDir>


SpellerDumper::SpellerDumper()
{

}

SpellerDumper::~SpellerDumper(){
    closeDumpingSession();
    delete parentDir;
}

void SpellerDumper::determineParentDirectory(QString suggestedPath){

    if(parentDir!=0){
        delete parentDir;
        parentDir=0;
    }

    parentDir = new QDir(suggestedPath);
    if(!parentDir->exists()){
        try{
            //risky, unproven...
            parentDir->mkpath(parentDir->absolutePath());
        }catch(std::exception& e){
            qWarning()<<"Failed to either find or create directory "<<parentDir->absolutePath()<<". Resorting to /tmp...";
            delete parentDir;
            parentDir = new QDir(QDir::tempPath());
        }
    }
}

void SpellerDumper::eegFrame(QSharedPointer<EegFrame> eegFrame){
    if(dataStream==0){
        qWarning()<<"EEG frame dumping attempted after file closing.";
    }else{
        *dataStream<<*eegFrame;
        dataStream->flush();
    }
}

// TODOs:
// - period number could be indicated as well

void SpellerDumper::spellerHighlight(short code){
    if(metaStream==0){
        qWarning()<<"Attempted write after file object destruction";
    }else{
        unsigned long time = Timer::getTime();
        //if the stimuli code corresponds to the current target. a bit of a (in)sanity check around the absolute values...
        if( (code<0 && qAbs(code)==qAbs(targetColumn)) || (code>0 && code==targetRow)){
            *metaStream<<time<<" 1  "<<code<<"\n";
        }else{
            *metaStream<<time<<" 0  "<<code<<"\n";
        }
        metaStream->flush();
    }
}

void SpellerDumper::spellerHint(short row, short column){
    targetRow=row;
    targetColumn=column;
}

void SpellerDumper::startDumpingSession(QString subjectName, QString parentDirPath){

    determineParentDirectory(parentDirPath);

    QString filename;
    unsigned short suffix = 0;

    do{
        filename=filename.sprintf("%s_session_%03d", subjectName.toStdString().c_str(), suffix++);
    }while(parentDir->exists(filename+"_data"));

    dataFile = new QFile(parentDir->filePath(filename+"_data"));
    metaFile = new QFile(parentDir->filePath(filename+"_meta"));
    targetsFile = new QFile(parentDir->filePath(filename+"_targets"));

    dataFile->open(QIODevice::WriteOnly | QIODevice::Text);
    metaFile->open(QIODevice::WriteOnly | QIODevice::Text);
    targetsFile->open(QIODevice::WriteOnly | QIODevice::Text);

    dataStream=new QTextStream(dataFile);
    metaStream=new QTextStream(metaFile);

    targetRow=0;
    targetColumn=0;
}

void SpellerDumper::closeDumpingSession(){
    if(dataStream!=0){
        dataStream->flush();
        delete dataStream;
    }
    if(metaStream!=0){
        metaStream->flush();
        delete metaStream;
    }
    if(targetsStream!=0){
        targetsStream->flush();
        delete targetsStream;
    }
    if(dataFile!=0){
        dataFile->close();
        delete dataFile;
    }
    if(metaFile!=0){
        metaFile->close();
        delete metaFile;
    }
    if(targetsFile!=0){
        targetsFile->close();
        delete targetsFile;
    }

    dataFile=metaFile=targetsFile=0;
    dataStream=metaStream=targetsStream=0;
}
