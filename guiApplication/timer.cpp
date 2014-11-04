#include "timer.h"
#include <QMutex>
#include <QDebug>

Timer* Timer::instance;

Timer::Timer() : QTime(){

}

Timer* Timer::getInstance(){
    static QMutex mutex;
    if(instance == NULL){
        mutex.lock();
        if(instance== NULL){
            instance=new Timer();
            ((QTime*)instance)->start();
        }
        mutex.unlock();
    }
    return instance;
}

/**
 * @brief Timer::start
 * SLightly deprecated. Now it is assumed that the first request top getTime() should result in starting the timer
 * (if needed, as well)
 */
void Timer::start(){
    Timer::getInstance();
}

int Timer::getTime(){
    return Timer::getInstance()->elapsed();
}


