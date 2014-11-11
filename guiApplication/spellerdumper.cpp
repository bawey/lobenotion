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
    *targetsStream<<Timer::getTime()<<" "<<row<<" "<<(-qAbs(column))<<endl;
}

void SpellerDumper::startDumpingSession(dataTakingParams* params){

    determineParentDirectory(params->parentDir);

    QString filename;
    unsigned short suffix = 0;

    do{
        filename=filename.sprintf("%s_session_%03d", params->subjectName.toStdString().c_str(), suffix++);
    }while(parentDir->exists(filename+"_data"));

    dataFile = new QFile(parentDir->filePath(filename+"_data"));
    metaFile = new QFile(parentDir->filePath(filename+"_meta"));
    targetsFile = new QFile(parentDir->filePath(filename+"_targets"));
    summaryFile = new QFile(parentDir->filePath(filename+"_summary"));

    dataFile->open(QIODevice::WriteOnly | QIODevice::Text);
    metaFile->open(QIODevice::WriteOnly | QIODevice::Text);
    targetsFile->open(QIODevice::WriteOnly | QIODevice::Text);
    summaryFile->open(QIODevice::WriteOnly | QIODevice::Text);

    dataStream=new QTextStream(dataFile);
    metaStream=new QTextStream(metaFile);
    targetsStream=new QTextStream(targetsFile);
    summaryStream=new QTextStream(summaryFile);

    *summaryStream<<"phrase: "<<params->phrase<<endl;
    *summaryStream<<"repeats: "<<params->epochsPerStimulus<<endl;
    *summaryStream<<"highlight: "<<params->stintHighlight<<endl;
    *summaryStream<<"dim: "<<params->stintDim<<endl;
    *summaryStream<<"info: "<<params->stintInfo<<endl;
    *summaryStream<<"interPeriod: "<<params->stintInterPeriod<<endl;

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
    if(summaryStream!=0){
        summaryStream->flush();
        delete summaryStream;
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
    if(summaryFile!=0){
        summaryFile->close();
        delete summaryFile;
    }

    dataFile=metaFile=targetsFile=summaryFile=0;
    dataStream=metaStream=targetsStream=summaryStream=0;
}

void SpellerDumper::spellerError(unsigned char errcode){
    //Ignore errors while not dumping
    if(summaryStream!=0){
       *summaryStream<<"errcode: "<<errcode<<" at "<<Timer::getTime()<<endl;
    }
}
