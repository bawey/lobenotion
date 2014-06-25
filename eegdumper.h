#ifndef EEGDUMPER_H
#define EEGDUMPER_H

#include <eegprocessor.h>
#include <eegframe.h>
#include <QSharedPointer>
#include <QFile>
#include <QTextStream>

class EegDumper : public EegProcessor
{
    Q_OBJECT
private:
    QFile file;
    QTextStream out;

public:
    explicit EegDumper();
    ~EegDumper();

signals:


public slots:
    void eegFrame(QSharedPointer<EegFrame> eegFrame);
};

#endif // EEGDUMPER_H
