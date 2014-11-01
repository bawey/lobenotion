#include "metaframe.h"
#include "timer.h"

MetaFrame::MetaFrame(
        unsigned char newCounter,
        char newGyroX,
        char newGyroY,
        unsigned char newBattery,
        float af3, float af4, float f3, float f4, float f7, float f8, float fc5, float fc6, float t7, float t8,
                               float p7, float p8, float o1, float o2) :
    counter(newCounter), gyroX(newGyroX), gyroY(newGyroY), battery(newBattery), timestamp(Timer::getTime()),
    quality(af3, af4, f3, f4, f7, f8, fc5, fc6, t7, t8, p7, p8, o1, o2)
{ }
