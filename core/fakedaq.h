#ifndef FAKEDAQ_H
#define FAKEDAQ_H
#include <eegdaq.h>

class FakeDaq : public EegDaq
{
public:
    FakeDaq();
    virtual void run();
private:
    float generateContactQuality(float time);
};

#endif // FAKEDAQ_H
