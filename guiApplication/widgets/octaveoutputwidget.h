#ifndef OCTAVEOUTPUTWIDGET_H
#define OCTAVEOUTPUTWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QString>
#include <QPlainTextEdit>

class OctaveOutputWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit OctaveOutputWidget(QWidget *parent = 0);

signals:

public slots:
    void slotOctaveOutput(QString output);
    void slotVScrollBarChanged(int);
private:
    QPlainTextEdit textEdit;
    bool sliderAllDown = false;
};

#endif // OCTAVEOUTPUTWIDGET_H
