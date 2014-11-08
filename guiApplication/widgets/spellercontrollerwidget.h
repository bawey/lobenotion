#ifndef SPELLERCONTROLLERWIDGET_H
#define SPELLERCONTROLLERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QSpinBox>

class SpellerControllerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpellerControllerWidget(QWidget *parent = 0);

private:
    QPushButton* daqStart;
    QPushButton* daqStop;
    QLabel* daqSignalProblem;

    // container for all the experiment parameters passed from the controller
    QFormLayout* formLayout;
    QWidget* form;
    // speller parameters
    QLineEdit* spellPhrase;
    QLineEdit* parentDir;
    QLineEdit* subjectName;

    QSpinBox* highlightDuration;
    QSpinBox* dimDuration;
    QSpinBox* infoDuration;
    QSpinBox* epochsPerStimulus;
    QSpinBox* interPeriodGap;

    void connectSignalsToSlots();

signals:
    void signalDataTakingStart(QString phrase, int epochsPerStimuli, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,
                               QString subjectName, QString parentDirectory);
    void signalDataTakingEnd();
public slots:
    void slotDataTakingStarted();
    void slotDataTakingFinished();
    // to handle the buttons
    void slotButtonPressedStart();
};

#endif // SPELLERCONTROLLERWIDGET_H
