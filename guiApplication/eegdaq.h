#ifndef EEGDAQ_H
#define EEGDAQ_H

#include <QObject>
#include <vector>
#include <QSharedPointer>
#include <metaframe.h>
#include <eegframe.h>
#include <QThread>
#include <QWaitCondition>

class EegDaq : public QThread
{
    Q_OBJECT

private:
    QWaitCondition* frameEmitted;
protected:
    bool _shutdown=false;
public:
    EegDaq();
    QWaitCondition* const getFrameEmittedWaitCondition() const;
signals:
    void eegFrame(QSharedPointer<EegFrame> eegFrame);
    void metaFrame(QSharedPointer<MetaFrame> metaFrame);

public slots:
    void frameProducedSlot(){
    // qDebug("this is important. means we can get frame notifications in the DAQ");
        frameEmitted->wakeAll();
    }
    void shutdown();
};

#endif // EEGDAQ_H
