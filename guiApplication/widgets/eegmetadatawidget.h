#ifndef EEGMETADATAWIDGET_H
#define EEGMETADATAWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSharedPointer>
#include <eegframe.h>
#include <metaframe.h>

class EegMetaDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EegMetaDataWidget(QWidget *parent = 0);
    ~EegMetaDataWidget();

private:
    QLabel* qualityLabels[EegFrame::CONTACTS_NO];
    QLabel signalQualityLabel;
    QLabel batteryLevelWidget;

signals:

public slots:
    void metaFrame(QSharedPointer<MetaFrame> framePtr);
    void signalGood(bool good);
};

#endif // EEGMETADATAWIDGET_H
