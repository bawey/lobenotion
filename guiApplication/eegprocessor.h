#ifndef EEGPROCESSOR_H
#define EEGPROCESSOR_H

#include <QSharedPointer>
#include <eegframe.h>
#include <metaframe.h>
#include <QThread>

class EegProcessor : public QThread
{
    Q_OBJECT

public:
    EegProcessor();

public slots:
    virtual void eegFrame(QSharedPointer<EegFrame> eegFrame);

};

#endif // EEGPROCESSOR_H
