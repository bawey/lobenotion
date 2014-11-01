#include "mainwindow.h"

MainWindow::MainWindow(EegVisualizer* visualizerObject,    MetaProcessor* metaProcessorObject,
                       SpellerDumper* spellerDumperObj,    EegDumper* eegDumperObject,
                       SpellerWidget* spellerWidgetObject, EegDaq* eegDaq,
                       QWidget *parent) :
    QMainWindow(parent), visualizer(visualizerObject), metaProcessor(metaProcessorObject),
    spellerDumper(spellerDumperObj), eegDumper(eegDumperObject), spellerWidget(spellerWidgetObject), daq(eegDaq)
{
    fileMenu=new QMenu();
    fileMenu->setTitle("File");
    takeData=new QAction(QString("Take data"), fileMenu);
    QObject::connect(takeData, SIGNAL(triggered()), this, SLOT(slotDataTaking()));
    QObject::connect(takeData, SIGNAL(triggered()), metaProcessor, SLOT(disableGraphicOutput()));
    overview=new QAction("Dashboard", fileMenu);
    QObject::connect(overview, SIGNAL(triggered()), this, SLOT(slotDashboard()));
    QObject::connect(overview, SIGNAL(triggered()), metaProcessor, SLOT(enableGraphicOutput()));

    fileMenu->addAction(overview);
    fileMenu->addAction(takeData);

    menuBar()->addMenu(fileMenu);
    editMenu=new QMenu();
    editMenu->setTitle("Edit");
    preferences=new QAction("Preferences", editMenu);
    QObject::connect(preferences, SIGNAL(triggered()), this, SLOT(slotPreferences()));
    editMenu->addAction(preferences);
    menuBar()->addMenu(editMenu);

    helpMenu=new QMenu();
    helpMenu->setTitle("Help");
    about=new QAction("About", helpMenu);
    QObject::connect(about, SIGNAL(triggered()), this, SLOT(slotAbout()));
    helpMenu->addAction(about);
    menuBar()->addMenu(helpMenu);

    this->setWindowTitle("Lobenotion 0.1");

    /* if the stuff was initialized elsewhere */
    if(visualizer==NULL){
        visualizer=new EegVisualizer();
    }if(metaProcessor==NULL){
        metaProcessor=new MetaProcessor();
    }if(eegDumper==NULL){
        eegDumper=new EegDumper();
    }if(spellerWidget==NULL){
        spellerWidget=new SpellerWidget();
    }if(spellerDumper==NULL){
        spellerDumper=new SpellerDumper();
    }

    this->setCentralWidget(new QWidget());

    stackLayout=new QStackedLayout();
    this->centralWidget()->setLayout(stackLayout);

    dashboardLayout = new QHBoxLayout();
    dashboardLayout->addWidget(visualizer->getWidget());
    dashboardLayout->addWidget(metaProcessor->getWidget());
    QWidget* dashboardWidget = new QWidget(centralWidget());
    dashboardWidget->setLayout(dashboardLayout);
    stackLayout->addWidget(dashboardWidget);

    QWidget* spellerWrapperWidget = new QWidget();
    QHBoxLayout* spellerBox = new QHBoxLayout();
    //without these stretches, the keyboard resizes nicely
    //spellerBox->addStretch();
    spellerWrapperWidget->setLayout(spellerBox);
    spellerWrapperWidget->layout()->addWidget(spellerWidget);
    //spellerBox->addStretch();
    QWidget* spellerButtonsWidget = new QWidget();
    spellerButtonsWidget->setLayout(new QVBoxLayout());

    daqStart=new QPushButton("Start");
    QObject::connect(daqStart, SIGNAL(clicked()), this, SLOT(slotDaqStart()));

    daqStop=new QPushButton("Finish");
    QObject::connect(daqStop, SIGNAL(clicked()), this, SLOT(slotDaqStop()));
    QObject::connect(daqStop, SIGNAL(clicked()), spellerWidget, SLOT(trainingResetFramesCount()));
    daqStop->setEnabled(false);

    daqSignalProblem = new QLabel();

    spellerButtonsWidget->layout()->addWidget(daqStart);
    spellerButtonsWidget->layout()->addWidget(daqStop);
    spellerButtonsWidget->layout()->addWidget(daqSignalProblem);
    spellerWrapperWidget->layout()->addWidget(spellerButtonsWidget);

    stackLayout->addWidget(spellerWrapperWidget);
    stackLayout->addWidget(new QLabel("3"));
    stackLayout->addWidget(new QLabel("4"));
    stackLayout->addWidget(new QLabel("5"));




    stackLayout->setCurrentIndex(5);



    /* connecting here after all */

    /** register signal datatypes and connect daq to processors **/
    qRegisterMetaType<QSharedPointer<EegFrame>>("QSharedPointer<EegFrame>");

    /** start MetaProcessor **/
    qRegisterMetaType<QSharedPointer<MetaFrame>>("QSharedPointer<MetaFrame>");



    /** these can be static: no highlights happen outside data taking **/
    QObject::connect(spellerWidget, SIGNAL(hint(int,int)), spellerDumper, SLOT(spellerHint(int,int)));
    QObject::connect(spellerWidget, SIGNAL(highlight(int, long)), spellerDumper, SLOT(spellerHighlight(int, long)));

    /** meta processor needs to keep running and checking data quality! **/
    QObject::connect(daq, SIGNAL(metaFrame(QSharedPointer<MetaFrame>)), metaProcessor, SLOT(metaFrame(QSharedPointer<MetaFrame>)));

    /** connect the window to data dumpers **/
    QObject::connect(this, SIGNAL(startDumpingSession()), eegDumper, SLOT(startDumpingSession()));
    QObject::connect(this, SIGNAL(startDumpingSession()), spellerDumper, SLOT(startDumpingSession()));
    QObject::connect(this, SIGNAL(closeDumpingSession()), eegDumper, SLOT(closeDumpingSession()));
    QObject::connect(this, SIGNAL(closeDumpingSession()), spellerDumper, SLOT(closeDumpingSession()));

    //connect by default at startup to dashboard
    slotDashboard();
}


void MainWindow::slotDataTaking(){
    /** disconnect dashboard from the signals it may receive **/
    QObject::disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), visualizer, SLOT(eegFrame(QSharedPointer<EegFrame>)));

    /** connect DAQ widgets **/

    /** switch panes **/
    stackLayout->setCurrentIndex(1);
}

void MainWindow::slotDashboard(){
    QObject::connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), visualizer, SLOT(eegFrame(QSharedPointer<EegFrame>)));

    stackLayout->setCurrentIndex(0);
}

void MainWindow::slotAbout(){
    stackLayout->setCurrentIndex(2);
}

void MainWindow::slotPreferences(){
    stackLayout->setCurrentIndex(3);
}

void MainWindow::slotDaqStart(){
    menuBar()->setEnabled(false);
    daqStart->setEnabled(false);
    daqStop->setEnabled(true);
    emit startDumpingSession();
    QObject::connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegDumper, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    QObject::connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), spellerWidget, SLOT(eegFrameNotification()));
    QObject::connect(metaProcessor, SIGNAL(signalFine(bool)), this, SLOT(signalFineDuringDaq(bool)));
    daqSignalProblem->hide();
}

void MainWindow::slotDaqStop(){
    menuBar()->setEnabled(true);
    daqStart->setEnabled(true);
    daqStop->setEnabled(false);
    spellerWidget->trainingResetFramesCount();
    QObject::disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), eegDumper, SLOT(eegFrame(QSharedPointer<EegFrame>)));
    QObject::disconnect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), spellerWidget, SLOT(eegFrameNotification()));
    QObject::disconnect(metaProcessor, SIGNAL(signalFine(bool)), this, SLOT(signalFineDuringDaq(bool)));
    emit closeDumpingSession();
}

void MainWindow::signalFineDuringDaq(bool yesno){
    if(!yesno){
        qDebug()<<"Noisy signal. Data taking stopping.";
        daqSignalProblem->setText("Noisy signal. Data taking stopped.");
        slotDaqStop();
        daqSignalProblem->show();
    }else{
    }
}
