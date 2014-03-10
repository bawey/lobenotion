#include "mainwindow.h"
#include <QApplication>
#include "qemokitd.h"
#include <QLCDNumber>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QEmokitD emokitd;

    MainWindow w(&emokitd);
    w.show();

    emokitd.start();






    return a.exec();
}
