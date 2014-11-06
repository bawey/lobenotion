#ifndef SPELLERCONTROLLERWIDGET_H
#define SPELLERCONTROLLERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class SpellerControllerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpellerControllerWidget(QWidget *parent = 0);

private:
    QPushButton* daqStart;
    QPushButton* daqStop;
    QLabel* daqSignalProblem;

    void connectSignalsToSlots();

signals:
    void signalDataTakingStart();
    void signalDataTakingEnd();
public slots:
    void slotDataTakingStarted();
    void slotDataTakingFinished();
};

#endif // SPELLERCONTROLLERWIDGET_H
