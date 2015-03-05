#ifndef EPOCDAQ_H
#define EPOCDAQ_H
#include <eegdaq.h>
#include <emokit/emokit.h>
#include <settings.h>

class EpocDaq : public EegDaq
{
public:
    EpocDaq();
    ~EpocDaq();
    virtual void run();

private:
    bool isDaemonRunning;
    struct emokit_device* epocDev;



};

#endif // EPOCDAQ_H
