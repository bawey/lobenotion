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
    QWidget* const getWidget() const{
        return widget;
    }

private:
    constexpr static int META_BUFFER_LENGTH=10;
    QSharedPointer<MetaFrame> metaBuffer[META_BUFFER_LENGTH];
    int metaBufferedCount = 0;
    QMutex lock;
    QWaitCondition bufferNotEmpty;
    QWaitCondition bufferNotFull;

    QWidget* widget;
    QLabel* qualityLabels[EegFrame::CONTACTS_NO];
    QLabel signalQualityLabel;
    QLabel batteryLevelWidget;
    QLabel metaBufferWidget;

    bool graphicOutputEnabled=true;
    bool signalsFineSoFar;

    void run();
    void processMetaFrame(QSharedPointer<MetaFrame>);

    static constexpr int processEvery=32;

signals:
    void signalFine(bool truefalse);
public slots:
    virtual void metaFrame(QSharedPointer<MetaFrame> eegFrame);
    void enableGraphicOutput(bool yesno=true);
    void disableGraphicOutput(){
        graphicOutputEnabled=false;
    }
};

#endif // METAPROCESSOR_H
