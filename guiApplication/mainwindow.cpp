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


    // Menus: File
    fileMenu=new QMenu();
    fileMenu->setTitle("File");

    takeData=new QAction(QString("Take data"), fileMenu);
    QObject::connect(takeData, SIGNAL(triggered()), this, SLOT(slotDataTaking()));
    fileMenu->addAction(takeData);

    dashboard=new QAction("Dashboard", fileMenu);
    QObject::connect(dashboard, SIGNAL(triggered()), this, SLOT(slotDashboard()));
    fileMenu->addAction(dashboard);

    menuBar()->addMenu(fileMenu);

    // Menus: Edit
    editMenu=new QMenu();
    editMenu->setTitle("Edit");

    preferences=new QAction("Preferences", editMenu);
    QObject::connect(preferences, SIGNAL(triggered()), this, SLOT(slotPreferences()));
    editMenu->addAction(preferences);

    menuBar()->addMenu(editMenu);

    // Menus: Help
    helpMenu=new QMenu();
    helpMenu->setTitle("Help");

    about=new QAction("About", helpMenu);
    QObject::connect(about, SIGNAL(triggered()), this, SLOT(slotAbout()));
    helpMenu->addAction(about);

    menuBar()->addMenu(helpMenu);

    // General stuff: title
    this->setWindowTitle("Lobenotion 0.2");

    // Central widget
    this->setCentralWidget(new QWidget());
    stackLayout=new QStackedLayout();
    this->centralWidget()->setLayout(stackLayout);

    // Dashboard (part of central widget)
    dashboardWidget = new QWidget(centralWidget());

    dashboardLayout = new QHBoxLayout();
    dashboardLayout->addWidget(eegPlot);
    dashboardLayout->addWidget(metaDataWidget);

    dashboardWidget->setLayout(dashboardLayout);
    stackLayout->addWidget(dashboardWidget);

    QWidget* spellerWrapperWidget = new QWidget();
    spellerBox = new QHBoxLayout();
    //without these stretches, the keyboard resizes nicely
    //spellerBox->addStretch();
    spellerWrapperWidget->setLayout(spellerBox);
    spellerWrapperWidget->layout()->addWidget(spellerWidget);
    spellerWrapperWidget->layout()->addWidget(spellerCtlWidget);
    //spellerBox->addStretch();


    stackLayout->addWidget(spellerWrapperWidget);
    stackLayout->addWidget(new QLabel("3"));
    stackLayout->addWidget(new QLabel("4"));
    stackLayout->addWidget(new QLabel("5"));

    stackLayout->setCurrentIndex(5);

    connectSignalsToSlots();

    slotDashboard();
}

void MainWindow::connectSignalsToSlots(){
    connect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaDataWidget, SLOT(metaFrame(QSharedPointer<MetaFrame>)));
    connect(metaProcessor, SIGNAL(signalFine(bool)), metaDataWidget, SLOT(signalGood(bool)));
    metaDataWidget->signalGood(metaProcessor->signalFine());
}


void MainWindow::slotDataTaking(){
    /** disconnect dashboard from the signals it may receive **/
    QObject::disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegPlot, SLOT(eegFrame(QSharedPointer<EegFrame>)));

    /** connect DAQ widgets **/

    /** switch panes **/
    stackLayout->setCurrentIndex(1);
}

void MainWindow::slotDashboard(){
    QObject::connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegPlot, SLOT(eegFrame(QSharedPointer<EegFrame>)));

    stackLayout->setCurrentIndex(0);
}

void MainWindow::slotAbout(){
    stackLayout->setCurrentIndex(2);
}

void MainWindow::slotPreferences(){
    stackLayout->setCurrentIndex(3);
}
