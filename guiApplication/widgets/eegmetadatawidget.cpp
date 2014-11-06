#include "eegmetadatawidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

EegMetaDataWidget::EegMetaDataWidget(QWidget *parent) :
    QWidget(parent)
{
    //TODO: investigate potential memory leaks and attempt to address 'em by parentage
    QVBoxLayout* column = new QVBoxLayout();
    for(int i=0; i<EegFrame::CONTACTS_NO; ++i){
        QHBoxLayout* row = new QHBoxLayout();
        QLabel* label = new QLabel(EegFrame::contacts[i].c_str());
        QLabel* value = new QLabel("---");
        row->addWidget(label);
        row->addWidget(value);
        qualityLabels[i]=value;
        column->addLayout(row);
    }

    QHBoxLayout* signalRow = new QHBoxLayout();
    signalRow->addWidget(new QLabel("Signal:"));
    signalRow->addWidget(&signalQualityLabel);
    signalQualityLabel.setText("Initializing...");
    column->addLayout(signalRow);

    QHBoxLayout* batteryRow = new QHBoxLayout();
    batteryRow->addWidget(new QLabel("Battery: "));
    batteryRow->addWidget(&batteryLevelWidget);
    batteryLevelWidget.setText("Initializing...");
    column->addLayout(batteryRow);

    QHBoxLayout* bufferRow = new QHBoxLayout();
    bufferRow->addWidget(new QLabel("Buffer:"));
    bufferRow->addWidget(&metaBufferWidget);
    column->addLayout(bufferRow);

    this->setLayout(column);
}

EegMetaDataWidget::~EegMetaDataWidget(){
    //TODO: delete all!!!!!
    for(unsigned char i=0; i<EegFrame::CONTACTS_NO; ++i){
        delete qualityLabels[i];
        qualityLabels[i]=0;
    }
}

void EegMetaDataWidget::metaFrame(QSharedPointer<MetaFrame> framePtr){
    for(int i=0; i<EegFrame::CONTACTS_NO; ++i){
        qualityLabels[i]->setText(QString::number(framePtr->getQuality(i),'f',3));
    }

    batteryLevelWidget.setText(QString::number(framePtr->battery));

}

void EegMetaDataWidget::signalGood(bool good){
    if(good){
        signalQualityLabel.setText("fine");
        signalQualityLabel.setStyleSheet("QLabel { color : green; }");
    }else{
        signalQualityLabel.setText("noisy");
        signalQualityLabel.setStyleSheet("QLabel { color : red; }");
    }
}
