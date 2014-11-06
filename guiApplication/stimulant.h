#ifndef STIMULANT_H
#define STIMULANT_H

#include <QThread>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <widgets/spellerwidget.h>
#include <eegdaq.h>

class Stimulant : public QThread
{
    Q_OBJECT
private:
    static constexpr int HIGHLIGHT_DURATION=1000;
    static constexpr int INTERVAL_DURATION=500;

    EegDaq* daq;
    bool trainingMode;

    void run();

public:
    explicit Stimulant(EegDaq* daq, bool trainingMode=true);

signals:
    void spellerHighlightRow(int r);
    void spellerHighlightColumn(int r);
    void spellerUnhighlight();
    void spellerRandomHint();


};

#endif // STIMULANT_H
