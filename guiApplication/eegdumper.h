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
    QString parentPath;
    QFile* file;
    QTextStream* out;

public:
    explicit EegDumper();
    ~EegDumper();

signals:


public slots:
    void eegFrame(QSharedPointer<EegFrame> eegFrame);
    void closeDumpingSession();
    void startDumpingSession();
};

#endif // EEGDUMPER_H
