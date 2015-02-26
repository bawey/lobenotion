#include "spellerdumper.h"
#include <timer.h>
#include <settings.h>
#include <QString>
#include <QDebug>
#include <QDir>


SpellerDumper::SpellerDumper()
{
    qRegisterMetaType<QSharedPointer<QVector<int>>>("QSharedPointer<QVector<int>>");
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
    if(isOnline){
        if(onlinePeriodOngoing){
            onlineData->append(Timer::getTime());
            for(int c=0; c<eegFrame->CONTACTS_NO; ++c){
                onlineData->append(eegFrame->getValue(c));
            }

            //a trick to prevent buffer underrun at the end of an epoch
            if(waitForCompleteEpoch>0){
                epochData->append(Timer::getTime());
                for(int c=0; c<eegFrame->CONTACTS_NO; ++c){
                    epochData->append(eegFrame->getValue(c));
                }

                if(--waitForCompleteEpoch==0){
                    emit signalOnlineEpochCaptured(epochData, epochMeta, epochTarg);
                }
            }
        }else{
            // it's not so abnormal - between the periods data keeps coming. a highlight would be more indicative
            // qWarning()<<"SpellerDumper: eegFrame received while in online mode, yet there is no ongoing period.";
        }
    }else{
        if(dataStream==0){
            qWarning()<<"EEG frame dumping attempted after file closing.";
        }else{
            *dataStream<<*eegFrame;
            dataStream->flush();
        }
    }
}

// TODOs:
// - period number could be indicated as well

void SpellerDumper::spellerHighlight(short code){
    if(isOnline){
        if(onlinePeriodOngoing){
            onlineMeta->append(Timer::getTime());
            onlineMeta->append(0);
            onlineMeta->append((int)code);
        }else{
            qWarning()<<"Speller hint received when dumper was on online mode, yet no period was ongoing";
        }
    }else{
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
}

void SpellerDumper::spellerHint(short row, short column){
    targetRow=row;
    targetColumn=column;
    *targetsStream<<Timer::getTime()<<" "<<row<<" "<<(-qAbs(column))<<endl;
}

void SpellerDumper::startDumpingSession(dataTakingParams* params){

    isOnline=false;

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

    *summaryStream<<"subject: "<<params->subjectName<<endl;
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

void SpellerDumper::startOnlineMode(const dataTakingParams* params){
    isOnline=true;
    //TODO: 12=2*6 hardcoded as number of distinct stimuli
    onlineMetaSize=3*params->epochsPerStimulus*12;
    //TODO: 14 hardcoded as the number of channels
    int estimatedTime = 12*params->epochsPerStimulus*(params->stintHighlight+params->stintDim)+params->stintInterPeriod*2;
    int estimatedFrames =qRound(estimatedTime*128.0/1000);
     onlineDataSize=estimatedFrames*(1+14);
    qDebug()<<"estimated FramesPerPeriod limit: "<<estimatedFrames<<", so the data size: "<<onlineDataSize<<" and the meta size: "<<onlineMetaSize;
}

void SpellerDumper::closeOnlineMode(){
    qDebug()<<"void SpellerDumper::closeOnlineMode()";
    isOnline=false;
    if(onlinePeriodOngoing){
        qDebug()<<"forced closing online period";
        closeOnlinePeriod();
    }
}

void SpellerDumper::startOnlinePeriod(){
    qDebug()<<"void SpellerDumper::startOnlinePeriod()";
    onlineData=new QVector<int>();
    onlineMeta=new QVector<int>();
    //There will only be one period and one meaningless target: 0,0
    onlineTrg=new QVector<int>();

    onlineTrg->append(Timer::getTime());
    onlineTrg->append(0);
    onlineTrg->append(0);

    onlinePeriodOngoing=true;
}

void SpellerDumper::closeOnlinePeriod(){
    qDebug()<<"void SpellerDumper::closeOnlinePeriod()";
    //Wrap the data and meta in QSharedPointers to be sent for processing and deferred deletion
    QSharedPointer<QVector<int>> shareData = QSharedPointer<QVector<int>>(onlineData);
    QSharedPointer<QVector<int>> shareMeta = QSharedPointer<QVector<int>>(onlineMeta);
    QSharedPointer<QVector<int>> sharedTrg = QSharedPointer<QVector<int>>(onlineTrg);

    onlinePeriodOngoing=false;
    emit onlinePeriodCaptured(shareData, shareMeta, sharedTrg);
}

// this one needs either to send pure pointers or to make a copy of passed objects.
// the latter solves the issue of data being appended simultaneously.
// however, it's very inefficient //copy after each period, decimate after each period...

//also, the moment a request is received there is not enough frames in the buffer following the last intensification

// maybe: store info that a period has ended.
// then count the frames until 120...

void SpellerDumper::slotOnlineEpochEnded(){
    this->waitForCompleteEpoch = 100;

    this->epochData = QSharedPointer<QVector<int>>(new QVector<int>(*onlineData));
    this->epochMeta = QSharedPointer<QVector<int>>(new QVector<int>(*onlineMeta));
    this->epochTarg = QSharedPointer<QVector<int>>(new QVector<int>(*onlineTrg));
}
