#include "spellerdumper.h"
#include <timer.h>
#include <settings.h>
#include <QString>
#include <QDebug>

SpellerDumper::SpellerDumper()
{

}

void SpellerDumper::spellerHighlight(int i, long time){
    if(out==0){
        qWarning()<<"Attempted write after file object destruction";
    }else{
        if( (i<0 && -1*(i+1)==nowColumn) || (i>0 && i-1==nowRow)){
            *out<<time<<" 1  "<<i<<"\n";
        }else{
            *out<<time<<" 0  "<<i<<"\n";
        }
        out->flush();
    }
}

void SpellerDumper::spellerHint(int row, int column){
    nowRow=row;
    nowColumn=column;
}

void SpellerDumper::startDumpingSession(){
    int suffix=1;
    QString path=Settings::getMetaDumpPath();
    while(QFile::exists(path)){
        path=Settings::getMetaDumpPath()+QString::number(suffix++);
    }
    file=new QFile(path);
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    out=new QTextStream(file);
    nowRow=-1;
    nowColumn=-1;
}

void SpellerDumper::closeDumpingSession(){
    out->flush();
    file->close();
    delete out;
    delete file;
    out=0;
    file=0;
}
