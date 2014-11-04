#ifndef TIMER_H
#define TIMER_H

#include <QTime>

class Timer : private QTime
{
private:
    static Timer* instance;
    Timer();

    static Timer* getInstance();
public:
    static void start();
    static int getTime();


};

#endif // TIMER_H
