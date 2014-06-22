#ifndef FAKEDAQ_H
#define FAKEDAQ_H
#include <eegdaq.h>

class FakeDaq : public EegDaq
{
public:
    FakeDaq();
    void run();
};

#endif // FAKEDAQ_H
