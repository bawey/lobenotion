#include "timer.h"
#include <QMutex>
#include <QDebug>

Timer* Timer::instance;

Timer::Timer() : QTime(){

}

void Timer::start(){
    static QMutex mutex;
    if(instance == NULL){
        mutex.lock();
        if(instance== NULL){
            instance=new Timer();
            ((QTime*)instance)->start();
        }
        mutex.unlock();
    }
}

int Timer::getTime(){
    return instance->elapsed();
}


