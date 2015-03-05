#include "mainwindow.h"
#include <QApplication>
#include <master.h>
#include <mainwindow.h>
#include <QFileDialog>
#include <QDebug>

/**
needs to start modules:
- DAQ
- Guis
- Stimulant
- Timer
**/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Master* master = Master::getInstance();
    /** high refactoring potential **/
    qDebug()<<"Master created";
    MainWindow* mainWindow = new MainWindow();
    qDebug()<<"Main window created";
    master->start();
    qDebug()<<"Master started";
    mainWindow->show();
    mainWindow->slotDashboard();
    qDebug()<<"Main window shown";
    return a.exec();
}
