#include "mockapplication.h"
#include <exception>
#include <iostream>
#include <QDebug>

MockApplication::MockApplication(int& argc, char ** argv) :
    QApplication(argc, argv)
{
}

bool MockApplication::notify(QObject *receiver_, QEvent *event_){
    try{
//        qDebug()<<"Using custom MockApplication::notify";
        return QApplication::notify(receiver_, event_);
    }catch(std::exception& e){
        std::cerr<<e.what();
    }
    return false;
}
