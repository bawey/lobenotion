#include "eegdaq.h"

EegDaq::EegDaq() :
    QThread()
{
}

void EegDaq::shutdown(){
    _shutdown=true;
}
