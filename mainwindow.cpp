#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLCDNumber>

MainWindow::MainWindow(QEmokitD * emokitd, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    emokitd(emokitd)
{
    ui->setupUi(this);


    QObject::connect(this->emokitd, &QEmokitD::dataRate,  ui->lcdNumber ,
                    static_cast<void (QLCDNumber::*)(double)> (&QLCDNumber::display));

}

MainWindow::~MainWindow()
{
    delete ui;
}

