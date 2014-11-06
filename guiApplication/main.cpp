#include "mainwindow.h"
#include <QApplication>
#include <master.h>
#include <mainwindow.h>

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
    Master* m = Master::getInstance();

    /** high refactoring potential **/
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

    return a.exec();
}
