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
#include <widgets/settingswidget.h>
#include <widgets/analysiswidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // CHILD WIDGETS:
    EegPlotWidget* eegPlot;
    SpellerWidget* spellerWidget;
    SpellerControllerWidget* spellerCtlWidget;
    EegMetaDataWidget* metaDataWidget;
    AnalysisWidget* analysisWidget;
    SettingsWidget* settingsWidget;

    // LAYOUT COMPONENTS:
    QStackedLayout* stackLayout;
    QWidget* dashboardWidget;
    QHBoxLayout* dashboardLayout;
    QHBoxLayout* spellerLayout;

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;

    // MENU ACTIONS:
    QAction* actTakeData;
    QAction* actDashboard;
    QAction* actPreferences;
    QAction* actAbout;
    QAction* actAnalyze;
    QAction* actReloadScripts;

    // OTHER PROGRAM COMPONENTS:
    Master* master;
    const EegDaq* daq = NULL;
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
    void show();
    void slotDataTaking();
    void slotOnlineUse();
    void slotDashboard();
    void slotAbout();
    void slotPreferences();
    void slotAnalyze();

    void slotDisplayError(QString);
    void slotNewDaq(const EegDaq*);

    void slotOctaveBusy(bool busy);
    void slotSessionOngoing(bool ongoing);

private:
    static const QString STATUS_OCTAVE_BUSY;
    static const QString STATUS_READY;
    static const QString STATUS_SESSION_ON;

    void connectDashboardSignals();
    void disconnectDashboardSignals();
    void status(QString status){
        statusBar()->showMessage(status);
    }

    void enableMenus(bool enable);
};

#endif // MAINWINDOW_H
