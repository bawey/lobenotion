#ifndef EEGDAQ_H
#define EEGDAQ_H

#include <QObject>
#include <vector>
#include <QSharedPointer>
#include <metaframe.h>
#include <eegframe.h>
#include <QThread>
#include <QWaitCondition>

class EegDaq : public QObject
{
    Q_OBJECT

private:
    QWaitCondition* frameEmitted;
protected:
    bool _shutdown=false;
public:
    EegDaq();
    QWaitCondition* getFrameEmittedWaitCondition() const;
    virtual void run() = 0;
signals:
    void eegFrame(QSharedPointer<EegFrame> eegFrame);
    void metaFrame(QSharedPointer<MetaFrame> metaFrame);

public slots:
    void slotLaunch();
    void frameProducedSlot(){
    // qDebug("this is important. means we can get frame notifications in the DAQ");
        frameEmitted->wakeAll();
    }
    void shutdown();
};

#endif // EEGDAQ_H
