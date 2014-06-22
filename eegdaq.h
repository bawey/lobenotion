#ifndef EEGDAQ_H
#define EEGDAQ_H

#include <QObject>
#include <vector>
#include <QSharedPointer>
#include <eegframe.h>
#include <metaframe.h>
#include <QThread>

class EegDaq : public QThread
{
    Q_OBJECT

private:

protected:
    bool _shutdown=false;
public:
    EegDaq();

signals:
    void eegFrame(QSharedPointer<EegFrame> eegFrame);
    void metaFrame(QSharedPointer<MetaFrame> metaFrame);

public slots:
    void shutdown();
};

#endif // EEGDAQ_H
