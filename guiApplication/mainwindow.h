#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QString>
#include <QWidget>
#include <eegvisualizer.h>
#include <spellerwidget.h>
#include <spellerdumper.h>
#include <metaprocessor.h>
#include <eegdumper.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <eegdaq.h>
#include <fakedaq.h>
#include <epocdaq.h>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    EegVisualizer*  visualizer;
    MetaProcessor*  metaProcessor;
    SpellerDumper*  spellerDumper;
    EegDumper*      eegDumper;
    SpellerWidget*  spellerWidget;
    EegDaq*         daq;


    QStackedLayout* stackLayout;
    QHBoxLayout* dashboardLayout;
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* helpMenu;
    QAction* takeData;
    QAction* overview;
    QAction* preferences;
    QAction* about;

    QPushButton* daqStart;
    QPushButton* daqStop;
    QLabel* daqSignalProblem;

public:
    explicit MainWindow(EegVisualizer* visualizerObject,    MetaProcessor* metaProcessorObject,
                        SpellerDumper* spellerDumperObj,    EegDumper* eegDumperObject,
                        SpellerWidget* spellerWidgetObject, EegDaq* eegDaq,
                        QWidget *parent = 0);

    QStackedLayout* getLayout(){
        return stackLayout;
    }



signals:
    void closeDumpingSession();
    void startDumpingSession();

public slots:
    void slotDataTaking();
    void slotDaqStart();
    void slotDaqStop();
    void slotDashboard();
    void slotAbout();
    void slotPreferences();
    void signalFineDuringDaq(bool yesno);
};

#endif // MAINWINDOW_H
