#ifndef EEGVISUALIZER_H
#define EEGVISUALIZER_H

#include <eegprocessor.h>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QMutexLocker>
#include <eegframe.h>
#include <metaframe.h>
#include <QWindow>
#include <QWidget>
#include <qcustomplot/qcustomplot.h>


class EegVisualizer : public EegProcessor
{
 Q_OBJECT

 public:
      EegVisualizer();
      QWidget* const getWidget() const{
        return widget;
      }

 private:
      constexpr static int EEG_BUFFER_LENGTH=10;

      QMutex lock;
      QWaitCondition bufferNotEmpty;
      QWaitCondition bufferNotFull;
      QSharedPointer<EegFrame> eegBuffer[EEG_BUFFER_LENGTH];

      int eegBufferedCount = 0;

      QWidget* widget;
      QCPGraph* graphs[EegFrame::CONTACTS_NO];

      void run();
      void processEegFrame(QSharedPointer<EegFrame> framePtr);

 public slots:
     void eegFrame(QSharedPointer<EegFrame> eegFrame);
};

#endif // EEGVISUALIZER_H
