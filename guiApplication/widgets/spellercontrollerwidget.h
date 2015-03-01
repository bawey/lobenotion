#ifndef SPELLERCONTROLLERWIDGET_H
#define SPELLERCONTROLLERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QStackedLayout>
#include <QDoubleSpinBox>
#include <settings.h>

class SpellerControllerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpellerControllerWidget(QWidget *parent = 0);

private:
    QPushButton* buttonStartDaq;
    QPushButton* buttonStopDaq;
    QLabel* daqSignalProblem;

    // container for all the experiment parameters passed from the controller
    QFormLayout* formLayout;
    QWidget* form;
    // offline speller parameters
    QLineEdit* spellPhrase;
    QLineEdit* parentDir;
    QLineEdit* subjectName;
    // online speller parameters
    QSpinBox* phraseLength;
    QComboBox* classifierCombo;
    QDoubleSpinBox* confidenceThreshold;
    QSpinBox* confidenceSetoffPeriod;
    QLabel* recognizedCharacters;

    QStackedLayout* modeStack;

    QSpinBox* highlightDuration;
    QSpinBox* dimDuration;
    QSpinBox* infoDuration;
    QSpinBox* epochsPerStimulus;
    QSpinBox* interPeriodGap;

    QLabel* labelError;

    void connectSignalsToSlots();
    void adaptButtonsState(bool inrun);

signals:
    void signalDataTakingStart(QString phrase, int epochsPerStimuli, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,
                               QString subjectName, QString parentDirectory);
    void signalDataTakingEnd();

    void signalOnlineModeStart(int charactersLimit, int epochsPerStimuli, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration);
    void signalOnlineModeEnd();
public slots:
    void slotSignalFine(bool);
    void slotDataTakingStarted();
    void slotDataTakingFinished();
    void slotSpellerError(unsigned char);
    // to handle the buttons
    void slotButtonPressedStart();
    void slotButtonPressedFinish();
    void switchOnline();
    void switchOffline();
    void slotRecognizedCharacter(QChar character);
    void slotRecognizedCharacter(QString charStr);
    void revalidate();
    void slotChangedConfidenceThreshold(double value){
        Settings::setConfidenceThreshold((float)value);
    }

    void slotChangedOnlineMinEpochs(int value){
        Settings::setOnlineMinEpochs(value);
    }
    void slotOnlineModeStart();
    void slotOnlineModeEnd();

    void configurationChanged();
};

#endif // SPELLERCONTROLLERWIDGET_H
