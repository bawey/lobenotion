#ifndef EPOCDAQ_H
#define EPOCDAQ_H
#include <eegdaq.h>
#include <emokit/emokit.h>

class EpocDaq : public EegDaq
{
public:
    EpocDaq();
    ~EpocDaq();

private:
    bool isDaemonRunning;
    struct emokit_device* epocDev;

    void run();


};

#endif // EPOCDAQ_H
