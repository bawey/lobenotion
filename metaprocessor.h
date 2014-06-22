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

class MetaProcessor : public QThread
{
    Q_OBJECT
public:
    explicit MetaProcessor();

private:
    constexpr static int META_BUFFER_LENGTH=10;
    QSharedPointer<MetaFrame> metaBuffer[META_BUFFER_LENGTH];
    int metaBufferedCount = 0;
    QMutex lock;
    QWaitCondition bufferNotEmpty;
    QWaitCondition bufferNotFull;

    QWidget widget;
    QLabel* qualityLabels[EegFrame::CONTACTS_NO];
    QLabel signalQualityLabel;
    QLabel batteryLevelWidget;
    QLabel metaBufferWidget;

    bool signalsFineSoFar;

    void run();
    void processMetaFrame(QSharedPointer<MetaFrame>);

signals:
    void signalFine(bool truefalse);
public slots:
    virtual void metaFrame(QSharedPointer<MetaFrame> eegFrame);
};

#endif // METAPROCESSOR_H
