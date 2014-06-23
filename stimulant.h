#ifndef STIMULANT_H
#define STIMULANT_H

#include <QThread>
#include <QWidget>
#include <QLabel>
#include <spellerwidget.h>

class Stimulant : public QThread
{
    Q_OBJECT
private:
    static constexpr int HIGHLIGHT_DURATION=1000;
    static constexpr int INTERVAL_DURATION=500;

    SpellerWidget speller;
    void run();

public:
    explicit Stimulant();

signals:
    void orderHighlightRow(int r);
    void orderHighlightColumn(int r);
    void orderUnhighlight();


};

#endif // STIMULANT_H
