#include "spellercontrollerwidget.h"
#include <settings.h>
#include <QDebug>
#include <spellercontroller.h>
#include <QMessageBox>
#include <master.h>
#include <QTreeView>
#include <QHeaderView>

SpellerControllerWidget::SpellerControllerWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);
    buttonStartDaq=new QPushButton("Start");
    buttonStartDaq->setEnabled(false);
    buttonStopDaq=new QPushButton("Finish");
    buttonStopDaq->setEnabled(false);
    daqSignalProblem = new QLabel();
    daqSignalProblem->setText("Cannot start data taking due to noisy signal.");
    daqSignalProblem->setWordWrap(true);
    daqSignalProblem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(buttonStartDaq);
    mainLayout->addWidget(buttonStopDaq);
    mainLayout->addWidget(daqSignalProblem);


    modeStack = new QStackedLayout();

    QFormLayout* onlineForm = new QFormLayout();
    QFormLayout* offlineForm = new QFormLayout();

    subjectName = new QLineEdit();
    subjectName->setText(Settings::getSubjectName());
    offlineForm->addRow("Subject name", subjectName);
    spellPhrase = new QLineEdit();
    spellPhrase->setText(Settings::getSpellerPhrase());
    offlineForm->addRow("Spell phrase", spellPhrase);
    parentDir=new QLineEdit();
    parentDir->setText(Settings::getEegDumpPath());
    offlineForm->addRow("Parent dir", parentDir);

    phraseLength = new QSpinBox();
    phraseLength->setMaximum(100);
    onlineForm->addRow("Phrase length", phraseLength);

    classifierCombo = new QComboBox();
    QTreeView* tv = new QTreeView();
    ClassifiersModel* classifiersModel = Master::getInstance()->getClassifiersModel();
    tv->setModel(classifiersModel);
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setSelectionMode(QAbstractItemView::SingleSelection);
    tv->header()->hide();
    classifierCombo->setModel(classifiersModel);
    classifierCombo->setView(tv);
    classifierCombo->view()->setMinimumWidth(666);
    onlineForm->addRow("Classifier", classifierCombo);

    confidenceSetoffPeriod=new QSpinBox();
    confidenceThreshold=new QDoubleSpinBox();
    confidenceThreshold->setMinimum(0.0);
    confidenceThreshold->setMaximum(1.0);
    confidenceThreshold->setSingleStep(0.01);
    confidenceThreshold->setAccelerated(true);

    onlineForm->addRow("Confidence thr.", confidenceThreshold);
    onlineForm->addRow("Min repeats", confidenceSetoffPeriod);

    recognizedCharacters = new QLabel();
    onlineForm->addRow("Decoded", recognizedCharacters);

    // might raise problems with types or create a loop, but we'll see
    connect(classifiersModel, SIGNAL(signalCurrentClassifierChanged(int)), classifierCombo, SLOT(setCurrentIndex(int)));
    connect(classifierCombo, SIGNAL(currentIndexChanged(int)), classifiersModel, SLOT(slotSetCurrentClassifier(int)));

    QWidget* onlineContainer = new QWidget();
    onlineContainer->setLayout(onlineForm);

    QWidget* offlineContainer = new QWidget();
    offlineContainer->setLayout(offlineForm);

    modeStack->addWidget(offlineContainer);
    modeStack->addWidget(onlineContainer);
    modeStack->setSizeConstraint(QLayout::SetMinimumSize);

    formLayout=new QFormLayout();
    form=new QWidget();
    form->setLayout(formLayout);
    epochsPerStimulus = new QSpinBox();
    formLayout->addRow("Repetitions", epochsPerStimulus);
    highlightDuration = new QSpinBox();
    highlightDuration->setMaximum(1000);
    formLayout->addRow("Highlight stint", highlightDuration);
    dimDuration = new QSpinBox();
    dimDuration->setMaximum(1000);
    formLayout->addRow("Dim stint", dimDuration);
    infoDuration = new QSpinBox();
    infoDuration->setMaximum(10000);
    formLayout->addRow("Info stint", infoDuration);
    interPeriodGap = new QSpinBox();
    interPeriodGap->setMaximum(10000);
    formLayout->addRow("Inter-period gap", interPeriodGap);
    //void startDataTaking(int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,

    mainLayout->addWidget(form);
    mainLayout->addLayout(modeStack);
    mainLayout->addStretch(100);


    labelError = new QLabel();
    labelError->setWordWrap(true);

    mainLayout->addWidget(labelError);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    ((QVBoxLayout*) mainLayout)->addStretch();

    // CONNECT INTERNAL SLOTS
    connect(buttonStartDaq, SIGNAL(clicked()), this, SLOT(slotButtonPressedStart()));
    connect(buttonStopDaq, SIGNAL(clicked()), this, SLOT(slotButtonPressedFinish()));
    connect(confidenceThreshold, SIGNAL(valueChanged(double)), this, SLOT(slotChangedConfidenceThreshold(double)));
    connect(confidenceSetoffPeriod, SIGNAL(valueChanged(int)), this, SLOT(slotChangedOnlineMinEpochs(int)));

//    form->setStyleSheet("background-color: pink;");
//    offlineContainer->setStyleSheet("background-color: yellow;");
//    onlineContainer->setStyleSheet("background-color: blue;");

}

void SpellerControllerWidget::configurationChanged(){
//    qDebug()<<"Got the config-update signal";
}

void SpellerControllerWidget::connectSignalsToSlots(){
//    QObject::connect(daqStart, SIGNAL(clicked()), this, SLOT(slotDaqStart()));
//    QObject::connect(daqStop, SIGNAL(clicked()), this, SLOT(slotDaqStop()));
//    QObject::connect(daqStop, SIGNAL(clicked()), spellerWidget, SLOT(trainingResetFramesCount()));


}

void SpellerControllerWidget::slotSignalFine(bool isFine){
    QPalette palette = daqSignalProblem->palette();
    if(Settings::qcInterrupt() && buttonStartDaq->isEnabled() && !isFine && Settings::qcInterrupt()){
        QColor color(255,0,0,255);
        palette.setColor(daqSignalProblem->foregroundRole(), color);
        buttonStartDaq->setEnabled(false);
    }else if(!Settings::qcInterrupt() ||
             (!buttonStopDaq->isEnabled() && !buttonStartDaq->isEnabled() && isFine)){
        //if data taking not in progress and START button disabled due to bad signal
        QColor color(0,0,0,0);
        palette.setColor(daqSignalProblem->foregroundRole(), color);
        buttonStartDaq->setEnabled(true);
    }
    daqSignalProblem->setPalette(palette);
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

void SpellerControllerWidget::switchOnline(){
    modeStack->setCurrentIndex(1);
}

void SpellerControllerWidget::switchOffline(){
    modeStack->setCurrentIndex(0);
}

void SpellerControllerWidget::slotRecognizedCharacter(QChar character){
    recognizedCharacters->setText(recognizedCharacters->text().append(character).append(" "));
}

void SpellerControllerWidget::slotRecognizedCharacter(QString charStr){
    recognizedCharacters->setText(recognizedCharacters->text().append(charStr).append(" "));
}

void SpellerControllerWidget::revalidate(){
    highlightDuration->setValue(Settings::getSpellerHighlightStint());
    epochsPerStimulus->setValue(Settings::getSpellerEpochsPerStimulus());
    dimDuration->setValue(Settings::getSpellerDimStint());
    infoDuration->setValue(Settings::getSpellerInfoStint());
    interPeriodGap->setValue(Settings::getSpellerInterPeriodStint());
    confidenceSetoffPeriod->setValue(Settings::getOnlineMinEpochs());
    confidenceThreshold->setValue(Settings::getConfidenceThreshold());
}

void SpellerControllerWidget::slotOnlineModeStart(){
    adaptButtonsState(true);
}
void SpellerControllerWidget::slotOnlineModeEnd(){
    adaptButtonsState(false);
}

void SpellerControllerWidget::slotDataTakingStarted(){
    adaptButtonsState(true);
    if(labelError->isVisible()){
        labelError->hide();
    }
}

void SpellerControllerWidget::slotDataTakingFinished(){
    adaptButtonsState(false);
}

void SpellerControllerWidget::slotButtonPressedFinish(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Test", "This will terminate currrent session. Continue?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if( modeStack->currentIndex()==0 ){
            emit signalDataTakingEnd();
            qDebug() << "Offline session termination confirmed";
        } else{
            emit signalOnlineModeEnd();
            qDebug() << "Online session termination confirmed";
        }
    } else {
        qDebug() << "Session termination abandoned";
    }
}

void SpellerControllerWidget::slotButtonPressedStart(){

    int highlightDurationValue = highlightDuration->text().toInt();
    int interStimulusInterval = dimDuration->text().toInt()+highlightDurationValue;
    int flashesPerStimulus = epochsPerStimulus->text().toInt();
    int interPeriodGapValue = interPeriodGap->text().toInt();
    int infoDurationValue = infoDuration->text().toInt();

    Settings::setSpellerHighlightStint(highlightDurationValue);
    Settings::setSpellerDimStint(interStimulusInterval-highlightDurationValue);
    Settings::setSpellerInfoStint(infoDurationValue);
    Settings::setSpellerEpochsPerStimulus(flashesPerStimulus);
    Settings::setSpellerInterPeriodStint(interPeriodGapValue);

    if(modeStack->currentIndex()==0){
        QString subjectNameValue = subjectName->text();
        QString pathValue = parentDir->text();
        QString phraseValue = spellPhrase->text();


        Settings::setSubjectName(subjectNameValue);
        Settings::setEegDumpPath(pathValue);
        Settings::setSpellerPhrase(phraseValue);


        emit signalDataTakingStart(phraseValue, flashesPerStimulus, interStimulusInterval, interPeriodGapValue, highlightDurationValue, infoDurationValue, subjectNameValue, pathValue);
    } else if(modeStack->currentIndex()==1){
       // modeStack on index 1 signifies SpellerControllerWidget being in Online mode
        int charactersLimit = phraseLength->text().toInt();
        emit signalOnlineModeStart(charactersLimit ,flashesPerStimulus, interStimulusInterval, interPeriodGapValue,
                                   highlightDurationValue, infoDurationValue);
    }
}

void SpellerControllerWidget::adaptButtonsState(bool inrun){
    buttonStopDaq->setEnabled(inrun);

    buttonStartDaq->setEnabled(!inrun);
    subjectName->setEnabled(!inrun);
    parentDir->setEnabled(!inrun);
    spellPhrase->setEnabled(!inrun);
    highlightDuration->setEnabled(!inrun);
    dimDuration->setEnabled(!inrun);
    epochsPerStimulus->setEnabled(!inrun);
    interPeriodGap->setEnabled(!inrun);
    infoDuration->setEnabled(!inrun);
    phraseLength->setEnabled(!inrun);
    classifierCombo->setEnabled(!inrun);
    confidenceThreshold->setEnabled(!inrun);
    confidenceSetoffPeriod->setEnabled(!inrun);
}
