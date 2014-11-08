#include "spellercontrollerwidget.h"
#include <settings.h>
#include <QDebug>
#include <spellercontroller.h>
#include <QMessageBox>

SpellerControllerWidget::SpellerControllerWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    buttonStartDaq=new QPushButton("Start");
    buttonStartDaq->setEnabled(false);
    buttonStopDaq=new QPushButton("Finish");
    buttonStopDaq->setEnabled(false);
    daqSignalProblem = new QLabel();
    daqSignalProblem->setText("Cannot start data taking due to noisy signal.");
    this->layout()->addWidget(buttonStartDaq);
    this->layout()->addWidget(buttonStopDaq);
    this->layout()->addWidget(daqSignalProblem);

    formLayout=new QFormLayout();
    form=new QWidget();
    form->setLayout(formLayout);

    subjectName = new QLineEdit();
    subjectName->setText(Settings::getSubjectName());
    formLayout->addRow("Subject name", subjectName);
    spellPhrase = new QLineEdit();
    spellPhrase->setText(Settings::getSpellerPhrase());
    formLayout->addRow("Spell phrase", spellPhrase);
    parentDir=new QLineEdit();
    parentDir->setText(Settings::getEegDumpPath());
    formLayout->addRow("Parent dir", parentDir);

    epochsPerStimulus = new QSpinBox();
    epochsPerStimulus->setValue(Settings::getSpellerEpochsPerStimulus());
    formLayout->addRow("Epochs per stimulus", epochsPerStimulus);
    highlightDuration = new QSpinBox();
    highlightDuration->setMaximum(1000);
    highlightDuration->setValue(Settings::getSpellerHighlightStint());
    formLayout->addRow("Highlight duration", highlightDuration);
    dimDuration = new QSpinBox();
    dimDuration->setMaximum(1000);
    dimDuration->setValue(Settings::getSpellerDimStint());
    formLayout->addRow("Dim duration", dimDuration);
    infoDuration = new QSpinBox();
    infoDuration->setMaximum(10000);
    infoDuration->setValue(Settings::getSpellerInfoStint());
    formLayout->addRow("Info duration", infoDuration);
    interPeriodGap = new QSpinBox();
    interPeriodGap->setMaximum(10000);
    interPeriodGap->setValue(Settings::getSpellerInterPeriodStint());
    formLayout->addRow("Inter-period gap", interPeriodGap);


    //void startDataTaking(int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,

    this->layout()->addWidget(form);

    labelError = new QLabel();
    this->layout()->addWidget(labelError);

    ((QVBoxLayout*)this->layout())->addStretch();

    // CONNECT INTERNAL SLOTS
    connect(buttonStartDaq, SIGNAL(clicked()), this, SLOT(slotButtonPressedStart()));
    connect(buttonStopDaq, SIGNAL(clicked()), this, SLOT(slotButtonPressedFinish()));
}

void SpellerControllerWidget::connectSignalsToSlots(){
//    QObject::connect(daqStart, SIGNAL(clicked()), this, SLOT(slotDaqStart()));
//    QObject::connect(daqStop, SIGNAL(clicked()), this, SLOT(slotDaqStop()));
//    QObject::connect(daqStop, SIGNAL(clicked()), spellerWidget, SLOT(trainingResetFramesCount()));
}


void SpellerControllerWidget::slotDataTakingStarted(){
    buttonStartDaq->setEnabled(false);
    buttonStopDaq->setEnabled(true);
    if(labelError->isVisible()){
        labelError->hide();
    }
}

void SpellerControllerWidget::slotDataTakingFinished(){
    buttonStartDaq->setEnabled(true);
    buttonStopDaq->setEnabled(false);
}

void SpellerControllerWidget::slotButtonPressedStart(){
    QString subjectNameValue = subjectName->text();
    QString pathValue = parentDir->text();
    QString phraseValue = spellPhrase->text();
    int highlightDurationValue = highlightDuration->text().toInt();
    int interStimulusInterval = dimDuration->text().toInt()+highlightDurationValue;
    int flashesPerStimulus = epochsPerStimulus->text().toInt();
    int interPeriodGapValue = interPeriodGap->text().toInt();
    int infoDurationValue = infoDuration->text().toInt();

    Settings::setSubjectName(subjectNameValue);
    Settings::setEegDumpPath(pathValue);
    Settings::setSpellerPhrase(phraseValue);

    Settings::setSpellerHighlightStint(highlightDurationValue);
    Settings::setSpellerDimStint(interStimulusInterval-highlightDurationValue);
    Settings::setSpellerInfoStint(infoDurationValue);
    Settings::setSpellerEpochsPerStimulus(flashesPerStimulus);
    Settings::setSpellerInterPeriodStint(interPeriodGapValue);

    emit signalDataTakingStart(phraseValue, flashesPerStimulus, interStimulusInterval, interPeriodGapValue, highlightDurationValue, infoDurationValue, subjectNameValue, pathValue);
}

void SpellerControllerWidget::slotSignalFine(bool isFine){
    if(buttonStartDaq->isEnabled() && !isFine){
        daqSignalProblem->show();
        buttonStartDaq->setEnabled(false);
    }else if(!buttonStopDaq->isEnabled() && !buttonStartDaq->isEnabled() && isFine){ //if data taking not in progress and START button disabled due to bad signal
        daqSignalProblem->hide();
        buttonStartDaq->setEnabled(true);
    }
}

void SpellerControllerWidget::slotSpellerError(unsigned char code){
    this->labelError->show();
    switch(code){
    case SpellerController::ERRCODE_SIGNAL:
        labelError->setText("Speller Ctl error: noisy signal!");
        break;
    case SpellerController::ERRCODE_PARAMETERS:
        labelError->setText("Speller Ctl error: parameters validation!");
        break;
    case SpellerController::ERRCODE_ABORTED:
        labelError->setText("Data taking aborted by the user.");
        break;
    default:
        labelError->setText(QString("Speller Ctl error code: %1!").arg(QString::number(code)));
        break;
    }
    qDebug()<<"Speller ERROR number "<<code;
}

void SpellerControllerWidget::slotButtonPressedFinish(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Test", "This will terminate the data taking session. Continue?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit signalDataTakingEnd();
        qDebug() << "Yes was clicked";
    } else {
        qDebug() << "Yes was *not* clicked";
    }
}
