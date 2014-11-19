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
    this->layout()->addWidget(buttonStartDaq);
    this->layout()->addWidget(buttonStopDaq);
    this->layout()->addWidget(daqSignalProblem);


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

    QWidget* onlineContainer = new QWidget();
    onlineContainer->setLayout(onlineForm);

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

    recognizedCharacters = new QLabel();
    onlineForm->addRow("Decoded", recognizedCharacters);

    // might raise problems with types or create a loop, but we'll see
    connect(classifiersModel, SIGNAL(signalCurrentClassifierChanged(int)), classifierCombo, SLOT(setCurrentIndex(int)));
    connect(classifierCombo, SIGNAL(currentIndexChanged(int)), classifiersModel, SLOT(slotSetCurrentClassifier(int)));

    QWidget* offlineContainer = new QWidget();
    offlineContainer->setLayout(offlineForm);

    modeStack->addWidget(offlineContainer);
    modeStack->addWidget(onlineContainer);
    modeStack->setSizeConstraint(QLayout::SetFixedSize);

    mainLayout->addLayout(modeStack);

    formLayout=new QFormLayout();
    form=new QWidget();
    form->setLayout(formLayout);
    epochsPerStimulus = new QSpinBox();
    epochsPerStimulus->setValue(Settings::getSpellerEpochsPerStimulus());
    formLayout->addRow("Repetitions", epochsPerStimulus);
    highlightDuration = new QSpinBox();
    highlightDuration->setMaximum(1000);
    highlightDuration->setValue(Settings::getSpellerHighlightStint());
    formLayout->addRow("Highlight stint", highlightDuration);
    dimDuration = new QSpinBox();
    dimDuration->setMaximum(1000);
    dimDuration->setValue(Settings::getSpellerDimStint());
    formLayout->addRow("Dim stint", dimDuration);
    infoDuration = new QSpinBox();
    infoDuration->setMaximum(10000);
    infoDuration->setValue(Settings::getSpellerInfoStint());
    formLayout->addRow("Info stint", infoDuration);
    interPeriodGap = new QSpinBox();
    interPeriodGap->setMaximum(10000);
    interPeriodGap->setValue(Settings::getSpellerInterPeriodStint());
    formLayout->addRow("Inter-period gap", interPeriodGap);

    //void startDataTaking(int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,

    this->layout()->addWidget(form);

    labelError = new QLabel();
    labelError->setWordWrap(true);

    this->layout()->addWidget(labelError);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    ((QVBoxLayout*) this->layout())->addStretch();

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
        qDebug()<<"We don't yet know how to start online data taking, but working on it :)";
        int charactersLimit = phraseLength->text().toInt();
        emit signalOnlineModeStart(charactersLimit ,flashesPerStimulus, interStimulusInterval, interPeriodGapValue,
                                   highlightDurationValue, infoDurationValue);
    }
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

void SpellerControllerWidget::switchOnline(){
    modeStack->setCurrentIndex(1);
}

void SpellerControllerWidget::switchOffline(){
    modeStack->setCurrentIndex(0);
}

void SpellerControllerWidget::slotRecognizedCharacter(QChar character){
    recognizedCharacters->setText(recognizedCharacters->text().append(character));
}

void SpellerControllerWidget::slotRecognizedCharacter(QString charStr){
    recognizedCharacters->setText(recognizedCharacters->text().append(charStr));
}
