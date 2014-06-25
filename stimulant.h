#ifndef STIMULANT_H
#define STIMULANT_H

#include <QThread>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <spellerwidget.h>

class Stimulant : public QThread
{
    Q_OBJECT
private:
    static constexpr int HIGHLIGHT_DURATION=1000;
    static constexpr int INTERVAL_DURATION=500;

    bool trainingMode;

    SpellerWidget speller;
    void run();

public:
    explicit Stimulant(bool trainingMode=true);

signals:
    void spellerHighlightRow(int r);
    void spellerHighlightColumn(int r);
    void spellerUnhighlight();
    void spellerRandomHint();


};

#endif // STIMULANT_H
