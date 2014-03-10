#ifndef QEMOKITD_H
#define QEMOKITD_H

#include <QThread>
#include "emokit/emokit.h"

class QEmokitD : public QThread
{
    Q_OBJECT

private:
    bool isDaemonRunning;
    struct emokit_device* d;
    int devicesCount;

    void setDevicesCount(int newCount);


public:
    QEmokitD();
    ~QEmokitD();
    void run();
    void setRunning(bool onOff);
    inline bool isRunning(){return isDaemonRunning;}
    inline int getDevicesCount() {return devicesCount;}

signals:
    void resumed();
    void paused();
    void devicesCountChanged(int devicesNo);
    void dataRate(double rate);
    void contactQualities();
    void sensorReadings();
    void batteryLevel();
};

#endif // QEMOKITD_H
