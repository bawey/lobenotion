#ifndef TIMER_H
#define TIMER_H

#include <QTime>

class Timer : private QTime
{
private:
    static Timer* instance;
    Timer();
public:
    static void start();
    static int getTime();


};

#endif // TIMER_H
