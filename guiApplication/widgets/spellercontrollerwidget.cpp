#include "spellercontrollerwidget.h"
#include <settings.h>


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
    ((QVBoxLayout*)this->layout())->addStretch();

    // CONNECT INTERNAL SLOTS
    connect(daqStart, SIGNAL(clicked()), this, SLOT(slotButtonPressedStart()));
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
