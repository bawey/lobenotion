#ifndef METAPROCESSOR_H
#define METAPROCESSOR_H
#include <metaframe.h>
#include <QThread>
#include <QSharedPointer>
#include <QMutex>
#include <QWaitCondition>
#include <QWidget>
#include <QLabel>
#include <eegframe.h>

class MetaProcessor : public QObject
{
    Q_OBJECT
public:
    explicit MetaProcessor();

    bool signalFine() const;

private:
    bool broadcasted = false;
    bool signalsFineSoFar;
    void processMetaFrame(QSharedPointer<MetaFrame>);

signals:
    void signalFine(bool isFine);
public slots:
    void metaFrame(QSharedPointer<MetaFrame> eegFrame);
    void configurationChanged();

};

#endif // METAPROCESSOR_H
