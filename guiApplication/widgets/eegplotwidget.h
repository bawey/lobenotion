#ifndef EEGPLOTWIDGET_H
#define EEGPLOTWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <eegframe.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qcustomplot.h>

class EegPlotWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit EegPlotWidget(QString title="Signals", QWidget *parent = 0);

private:
    QCPGraph* graphs[EegFrame::CONTACTS_NO];

signals:

public slots:
    void eegFrame(QSharedPointer<EegFrame> eegFrame);
};

#endif // EEGPLOTWIDGET_H
