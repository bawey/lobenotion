#include "spellerdumper.h"
#include <timer.h>

SpellerDumper::SpellerDumper() : file("/home/bawey/Desktop/labels"), out(&file)
{
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    nowRow=-1;
    nowColumn=-1;
}

void SpellerDumper::spellerHighlight(int i){
    if( (i<0 && -1*(i+1)==nowColumn) || (i>0 && i-1==nowRow)){
        out<<Timer::getTime()<<" 1  "<<i<<"\n";
    }else{
        out<<Timer::getTime()<<" 0  "<<i<<"\n";
    }
    out.flush();
}

void SpellerDumper::spellerHint(int row, int column){
    nowRow=row;
    nowColumn=column;
}
