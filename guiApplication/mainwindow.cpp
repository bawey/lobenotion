#include "mainwindow.h"
#include "widgets/eegmetadatawidget.h"
#include <QMessageBox>

const QString MainWindow::STATUS_OCTAVE_BUSY = "Octave running...";
const QString MainWindow::STATUS_READY = "Ready";
const QString MainWindow::STATUS_SESSION_ON = "Data taking in progress...";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // copy references to other modules
    master = Master::getInstance();
    spellerCtl = master->getSpellerController();
    metaProcessor = master->getMetaProcessor();

    // initialize child widgets (they are distributed slightly later)
    spellerWidget=new SpellerWidget();
    eegPlot=new EegPlotWidget();
    metaDataWidget = new EegMetaDataWidget();
    spellerCtlWidget= new SpellerControllerWidget();
    settingsWidget = new SettingsWidget();

    // Menus: File
    fileMenu=new QMenu();
    fileMenu->setTitle("File");

    actDashboard=new QAction("Dashboard", fileMenu);
    QObject::connect(actDashboard, SIGNAL(triggered()), this, SLOT(slotDashboard()));
    fileMenu->addAction(actDashboard);

    actTakeData=new QAction(QString("Take data"), fileMenu);
    QObject::connect(actTakeData, SIGNAL(triggered()), this, SLOT(slotDataTaking()));
    fileMenu->addAction(actTakeData);

    actAnalyze = new QAction(QString("Analyze"), fileMenu);
    QObject::connect(actAnalyze, SIGNAL(triggered()), this, SLOT(slotAnalyze()));
    fileMenu->addAction(actAnalyze);

    /**
      TODO: Temporary section to leapfrog into online - delete
    **/
    QAction* onlineShortcut = new QAction("Online", fileMenu);
    connect(onlineShortcut, SIGNAL(triggered()), this, SLOT(slotOnlineUse()));
    fileMenu->addAction(onlineShortcut);

    menuBar()->addMenu(fileMenu);

    // Menus: Edit
    editMenu=new QMenu();
    editMenu->setTitle("Edit");

    actPreferences=new QAction("Preferences", editMenu);
    QObject::connect(actPreferences, SIGNAL(triggered()), this, SLOT(slotPreferences()));
    editMenu->addAction(actPreferences);

    menuBar()->addMenu(editMenu);

    // Menus: Help
    helpMenu=new QMenu();
    helpMenu->setTitle("Help");

    actAbout=new QAction("About", helpMenu);
    QObject::connect(actAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
    helpMenu->addAction(actAbout);

    actReloadScripts = new QAction("Reload Octave scripts", helpMenu);
    QObject::connect(actReloadScripts, SIGNAL(triggered()), master->getOctaveProxy(), SLOT(slotReloadScripts()));
    helpMenu->addAction(actReloadScripts);

    menuBar()->addMenu(helpMenu);

    // General stuff: title
    this->setWindowTitle("Lobenotion 0.4");

    // Central widget
    this->setCentralWidget(new QWidget());
    stackLayout=new QStackedLayout();
    this->centralWidget()->setLayout(stackLayout);

    // Dashboard (part of central widget)
    dashboardWidget = new QWidget(centralWidget());

    dashboardLayout = new QHBoxLayout();
    dashboardLayout->addWidget(eegPlot);
    QVBoxLayout* metaWrapper = new QVBoxLayout();
    metaWrapper->addWidget(metaDataWidget);
    metaWrapper->addStretch();
    dashboardLayout->addLayout(metaWrapper);
    metaDataWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    dashboardWidget->setLayout(dashboardLayout);
    stackLayout->addWidget(dashboardWidget);

    QWidget* spellerWrapperWidget = new QWidget();
    spellerLayout = new QHBoxLayout();
    spellerWrapperWidget->setLayout(spellerLayout);
    spellerWrapperWidget->layout()->addWidget(spellerWidget);
    spellerWrapperWidget->layout()->addWidget(spellerCtlWidget);
    stackLayout->addWidget(spellerWrapperWidget);

    analysisWidget = new AnalysisWidget(this);
    stackLayout->addWidget(analysisWidget);

    stackLayout->addWidget(settingsWidget);
    stackLayout->addWidget(new QLabel("5: This program is..."));

    connectSignalsToSlots();

}

void MainWindow::connectSignalsToSlots(){
    connect(metaProcessor, SIGNAL(signalFine(bool)), metaDataWidget, SLOT(signalGood(bool)));
    metaDataWidget->signalGood(metaProcessor->signalFine());

    // spellerCtlWidget and spellerController - permanent connections
    connect(spellerCtlWidget, SIGNAL(signalDataTakingStart(QString,int,int,int,int,int,QString,QString)), spellerCtl, SLOT(startDataTaking(QString,int,int,int,int,int,QString,QString)));
    connect(spellerCtlWidget, SIGNAL(signalDataTakingEnd()), spellerCtl, SLOT(endDataTaking()));
    connect(spellerCtlWidget, SIGNAL(signalOnlineModeStart(int,int,int,int,int,int)), spellerCtl, SLOT(startOnline(int,int,int,int,int,int)));
    connect(spellerCtlWidget, SIGNAL(signalOnlineModeEnd()), spellerCtl, SLOT(endOnline()));
    connect(spellerCtl, SIGNAL(dataTakingStarted(dataTakingParams*)), spellerCtlWidget, SLOT(slotDataTakingStarted()));
    connect(spellerCtl, SIGNAL(dataTakingEnded()), spellerCtlWidget, SLOT(slotDataTakingFinished()));
    connect(spellerCtl, SIGNAL(onlineModeStarted(const dataTakingParams*)), spellerCtlWidget, SLOT(slotOnlineModeStart()));
    connect(spellerCtl, SIGNAL(onlineModeEnded()), spellerCtlWidget, SLOT(slotOnlineModeEnd()));
    connect(spellerCtl, SIGNAL(signalSymbolRecognized(QChar)), spellerCtlWidget, SLOT(slotRecognizedCharacter(QChar)));
    connect(spellerCtl, SIGNAL(signalSymbolRecognized(QString)), spellerCtlWidget, SLOT(slotRecognizedCharacter(QString)));


    connect(spellerCtl, SIGNAL(error(unsigned char)), spellerCtlWidget, SLOT(slotSpellerError(unsigned char)));
    // spellerCtlWidget needs also some notion of signal quality from MetaProcessor
    connect(metaProcessor, SIGNAL(signalFine(bool)), spellerCtlWidget, SLOT(slotSignalFine(bool)));
    spellerCtlWidget->slotSignalFine(metaProcessor->signalFine());
    // spellerWidget and spellerController - permanent connections
    connect(spellerCtl, SIGNAL(commandRowColHighlight(short)), spellerWidget, SLOT(highlight(short)));
    connect(spellerCtl, SIGNAL(commandDimKeyboard()), spellerWidget, SLOT(unhighlight()));
    connect(spellerCtl, SIGNAL(commandIndicateTarget(short,short)), spellerWidget, SLOT(highlightTile(short,short)));
    connect(spellerCtl, SIGNAL(commandShowMessage(QString)), spellerWidget, SLOT(showMessage(QString)));

    // spellerController to mainWindow - prevent switching tabs

    connect(master->getOctaveProxy(), SIGNAL(signalFetchedOutput(QString)), analysisWidget->octaveOutputWidget(), SLOT(slotOctaveOutput(QString)));

    //Passing sessions as test material: sessions manager widget -> classifiers manager widget
    connect(analysisWidget->sessionsManagerWidget(), SIGNAL(signalTestModel(QSharedPointer<QList<const P3SessionInfo*> >)),
            analysisWidget->classifiersManagerWidget(), SLOT(slotTakeSessionsForTest(QSharedPointer<QList<const P3SessionInfo*> >)));

    //Launching online mode from classifiers management panel
    connect(analysisWidget->classifiersManagerWidget(), SIGNAL(signalGoOnline()), this, SLOT(slotOnlineUse()));
    connect(master, SIGNAL(signalErrorRelay(QString)), this, SLOT(slotDisplayError(QString)));

    // Classes reacting to configuration changes
    connect(Settings::getInstance(), SIGNAL(configurationChanged()), spellerCtlWidget, SLOT(configurationChanged()));
    connect(Settings::getInstance(), SIGNAL(configurationChanged()), metaProcessor, SLOT(configurationChanged()),
             Qt::DirectConnection);

    connect(Settings::getInstance(), SIGNAL(configurationChanged(QString)), master, SLOT(slotConfigChanged(QString)));

    connect(master, SIGNAL(signalNewDaq(const EegDaq*)), this, SLOT(slotNewDaq(const EegDaq*)));
    connect(master, SIGNAL(signalNewDaq(const EegDaq*)), spellerCtl, SLOT(slotNewDaq(const EegDaq*)));

    connect(master->getOctaveProxy(), SIGNAL(signalOctaveBusy(bool)), this, SLOT(slotOctaveBusy(bool)));
    connect(master->getOctaveProxy(), SIGNAL(signalOctaveBusy(bool)), analysisWidget, SLOT(slotOctaveBusy(bool)));

    connect(spellerCtl, SIGNAL(signalSessionOngoing(bool)), this, SLOT(slotSessionOngoing(bool)));
}

void MainWindow::slotDashboard(){
    connectDashboardSignals();
    stackLayout->setCurrentIndex(0);
}

void MainWindow::slotDataTaking(){
    disconnectDashboardSignals();
    stackLayout->setCurrentIndex(1);
    spellerCtlWidget->switchOffline();
    spellerCtlWidget->revalidate();
}

void MainWindow::slotOnlineUse(){
    disconnectDashboardSignals();
    stackLayout->setCurrentIndex(1);
    spellerCtlWidget->switchOnline();
    spellerCtlWidget->revalidate();
}

void MainWindow::slotAnalyze(){
    disconnectDashboardSignals();
    stackLayout->setCurrentIndex(2);
}

void MainWindow::slotPreferences(){
    disconnectDashboardSignals();
    settingsWidget->revalidate();
    stackLayout->setCurrentIndex(3);
}

void MainWindow::slotAbout(){
    disconnectDashboardSignals();
    stackLayout->setCurrentIndex(4);
}

void MainWindow::slotDisplayError(QString errmsg){
    QMessageBox qmb;
    qmb.setModal(true);
    qmb.setText(errmsg);
    qmb.setIcon(QMessageBox::Icon::Warning);
    qmb.exec();
}

void MainWindow::slotNewDaq(const EegDaq * newDaq){
    qDebug()<<"MainWindow::settingNewDaq";
    this->daq = newDaq;
}

void MainWindow::connectDashboardSignals(){
    if(daq!=NULL){
        connect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaDataWidget, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
        connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegPlot, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    }
}

void MainWindow::disconnectDashboardSignals(){
    if( daq!=NULL ){
        disconnect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaDataWidget, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
        disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegPlot, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    }
}

void MainWindow::slotOctaveBusy(bool busy){
    status(busy ? STATUS_OCTAVE_BUSY : STATUS_READY);
    enableMenus(!busy);
}

void MainWindow::slotSessionOngoing(bool ongoing){
    status(ongoing ? STATUS_SESSION_ON : STATUS_READY);
    enableMenus(!ongoing);
}


void MainWindow::show(){
    QMainWindow::show();
    slotDashboard();
    status(MainWindow::STATUS_READY);
}

void MainWindow::enableMenus(bool enable){
    fileMenu->setEnabled(enable);
    editMenu->setEnabled(enable);
    helpMenu->setEnabled(enable);
}
