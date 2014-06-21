#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qemokitd.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QEmokitD * emokitd, QWidget *parent = 0);
    ~MainWindow();
    
private slots:


private:
    Ui::MainWindow *ui;
    QEmokitD * emokitd;
};

#endif // MAINWINDOW_H