#include "spellercontrollerwidget.h"

SpellerControllerWidget::SpellerControllerWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    daqStart=new QPushButton("Start");
    daqStop=new QPushButton("Finish");
    daqStop->setEnabled(false);
    daqSignalProblem = new QLabel();
    this->layout()->addWidget(daqStart);
    this->layout()->addWidget(daqStop);
    this->layout()->addWidget(daqSignalProblem);
}

void SpellerControllerWidget::connectSignalsToSlots(){
//    QObject::connect(daqStart, SIGNAL(clicked()), this, SLOT(slotDaqStart()));
//    QObject::connect(daqStop, SIGNAL(clicked()), this, SLOT(slotDaqStop()));
//    QObject::connect(daqStop, SIGNAL(clicked()), spellerWidget, SLOT(trainingResetFramesCount()));
}


void SpellerControllerWidget::slotDataTakingStarted(){
    daqStart->setEnabled(false);
    daqStop->setEnabled(true);

    daqSignalProblem->hide();
}

void SpellerControllerWidget::slotDataTakingFinished(){
    daqStart->setEnabled(true);
    daqStop->setEnabled(false);
}
