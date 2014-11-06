#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QString>
#include <QWidget>
#include <widgets/eegplotwidget.h>
#include <widgets/spellerwidget.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <master.h>
#include <widgets/eegmetadatawidget.h>
#include <widgets/spellercontrollerwidget.h>
#include <eegdaq.h>
#include <spellercontroller.h>
#include <metaprocessor.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // CHILD WIDGETS:
    EegPlotWidget* eegPlot;
    SpellerWidget* spellerWidget;
    SpellerControllerWidget* spellerCtlWidget;
    EegMetaDataWidget* metaDataWidget;

    // LAYOUT COMPONENTS:
    QStackedLayout* stackLayout;
    QWidget* dashboardWidget;
    QHBoxLayout* dashboardLayout;
    QHBoxLayout* spellerBox;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;

    // MENU ACTIONS:
    QAction* takeData;
    QAction* dashboard;
    QAction* preferences;
    QAction* about;

    // OTHER PROGRAM COMPONENTS:
    Master* master;
    const EegDaq* daq;
    const SpellerController* spellerCtl;
    const MetaProcessor* metaProcessor;

    void connectSignalsToSlots();

public:
    explicit MainWindow(QWidget *parent = 0);

    QStackedLayout* getLayout(){
        return stackLayout;
    }

signals:
    void closeDumpingSession();
    void startDumpingSession();

public slots:
    void slotDataTaking();
    void slotDashboard();
    void slotAbout();
    void slotPreferences();
};

#endif // MAINWINDOW_H
