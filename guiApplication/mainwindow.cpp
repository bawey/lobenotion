#include "mainwindow.h"
#include "widgets/eegmetadatawidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // copy references to other modules
    master = Master::getInstance();
    daq = master->getDaq();
    spellerCtl = master->getSpellerController();
    metaProcessor = master->getMetaProcessor();

    // initialize child widgets (they are distributed slightly later)
    spellerWidget=new SpellerWidget();
    eegPlot=new EegPlotWidget();
    metaDataWidget = new EegMetaDataWidget();
    spellerCtlWidget= new SpellerControllerWidget();
    sessionsWidget = new SessionsManagerWidget(master->getSessionsModel());
    octaveWidget = new OctaveOutputWidget();
    classifiersWidget = new ClassifiersManagerWidget();

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

    menuBar()->addMenu(helpMenu);

    // General stuff: title
    this->setWindowTitle("Lobenotion 0.3");

    // Central widget
    this->setCentralWidget(new QWidget());
    stackLayout=new QStackedLayout();
    this->centralWidget()->setLayout(stackLayout);

    // Dashboard (part of central widget)
    dashboardWidget = new QWidget(centralWidget());

    dashboardLayout = new QHBoxLayout();
    dashboardLayout->addWidget(eegPlot);
    dashboardLayout->addWidget(metaDataWidget);
    metaDataWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    dashboardWidget->setLayout(dashboardLayout);
    stackLayout->addWidget(dashboardWidget);

    QWidget* spellerWrapperWidget = new QWidget();
    spellerLayout = new QHBoxLayout();
    spellerWrapperWidget->setLayout(spellerLayout);
    spellerWrapperWidget->layout()->addWidget(spellerWidget);
    spellerWrapperWidget->layout()->addWidget(spellerCtlWidget);
    stackLayout->addWidget(spellerWrapperWidget);

    analysisWidget = new QWidget();
    analysisLayout = new QHBoxLayout();
    analysisWidget->setLayout(analysisLayout);
    analysisLayout->addWidget(sessionsWidget);
    analysisLayout->addWidget(octaveWidget);
    analysisLayout->addWidget(classifiersWidget);
    stackLayout->addWidget(analysisWidget);

    stackLayout->addWidget(new QLabel("4: Preferences?"));
    stackLayout->addWidget(new QLabel("5: This program is..."));

    stackLayout->setCurrentIndex(1);

    connectSignalsToSlots();

    slotDashboard();
}

void MainWindow::connectSignalsToSlots(){
    connect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaDataWidget, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
    connect(metaProcessor, SIGNAL(signalFine(bool)), metaDataWidget, SLOT(signalGood(bool)));
    metaDataWidget->signalGood(metaProcessor->signalFine());

    // spellerCtlWidget and spellerController - permanent connections
    connect(spellerCtlWidget, SIGNAL(signalDataTakingStart(QString,int,int,int,int,int,QString,QString)), spellerCtl, SLOT(startDataTaking(QString,int,int,int,int,int,QString,QString)));
    connect(spellerCtlWidget, SIGNAL(signalDataTakingEnd()), spellerCtl, SLOT(endDataTaking()));
    connect(spellerCtl, SIGNAL(dataTakingStarted(dataTakingParams*)), spellerCtlWidget, SLOT(slotDataTakingStarted()));
    connect(spellerCtl, SIGNAL(dataTakingEnded()), spellerCtlWidget, SLOT(slotDataTakingFinished()));
    connect(spellerCtl, SIGNAL(error(unsigned char)), spellerCtlWidget, SLOT(slotSpellerError(unsigned char)));
    // spellerCtlWidget needs also some notion of signal quality from MetaProcessor
    connect(metaProcessor, SIGNAL(signalFine(bool)), spellerCtlWidget, SLOT(slotSignalFine(bool)));
    spellerCtlWidget->slotSignalFine(metaProcessor->signalFine());
    // spellerWidget and spellerController - permanent connections
    connect(spellerCtl, SIGNAL(commandRowColHighlight(short)), spellerWidget, SLOT(highlight(short)));
    connect(spellerCtl, SIGNAL(commandDimKeyboard()), spellerWidget, SLOT(unhighlight()));
    connect(spellerCtl, SIGNAL(commandIndicateTarget(short,short)), spellerWidget, SLOT(highlightTile(short,short)));
    //connect(spellerCtl, SIGNAL(), spellerWidget, SLOT());

    // spellerController to mainWindow - prevent switching tabs

    connect(master->getOctaveProxy(), SIGNAL(signalFetchedOutput(QString)), octaveWidget, SLOT(slotOctaveOutput(QString)));
}

void MainWindow::slotDashboard(){
    QObject::connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegPlot, SLOT(eegFrame(QSharedPointer<EegFrame>)));

    stackLayout->setCurrentIndex(0);
}

void MainWindow::slotDataTaking(){
    /** disconnect dashboard from the signals it may receive **/
    QObject::disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegPlot, SLOT(eegFrame(QSharedPointer<EegFrame>)));

    /** connect DAQ widgets ... if any **/

    /** switch panes **/
    stackLayout->setCurrentIndex(1);
}

void MainWindow::slotAnalyze(){
    /** disconnect dashboard from the signals it may receive **/
    QObject::disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegPlot, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    stackLayout->setCurrentIndex(2);
}

void MainWindow::slotPreferences(){
    stackLayout->setCurrentIndex(3);
}

void MainWindow::slotAbout(){
    stackLayout->setCurrentIndex(4);
}


