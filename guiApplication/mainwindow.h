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
#include <widgets/sessionsmanagerwidget.h>
#include <widgets/octaveoutputwidget.h>
#include <widgets/classifiersmanagerwidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // CHILD WIDGETS:
    EegPlotWidget* eegPlot;
    SpellerWidget* spellerWidget;
    SpellerControllerWidget* spellerCtlWidget;
    EegMetaDataWidget* metaDataWidget;
    SessionsManagerWidget* sessionsWidget;
    OctaveOutputWidget* octaveWidget;
    ClassifiersManagerWidget* classifiersWidget;

    // LAYOUT COMPONENTS:
    QStackedLayout* stackLayout;
    QWidget* dashboardWidget;
    QHBoxLayout* dashboardLayout;
    QHBoxLayout* spellerLayout;
    QWidget* analysisWidget;
    QHBoxLayout* analysisLayout;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;

    // MENU ACTIONS:
    QAction* actTakeData;
    QAction* actDashboard;
    QAction* actPreferences;
    QAction* actAbout;
    QAction* actAnalyze;

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
    void slotOnlineUse();
    void slotDashboard();
    void slotAbout();
    void slotPreferences();
    void slotAnalyze();
private:
    void prologDataTakingOrOnline();

};

#endif // MAINWINDOW_H
