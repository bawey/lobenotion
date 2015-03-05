#include "eegplotwidget.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QPalette>
#include <QVBoxLayout>

EegPlotWidget::EegPlotWidget(QString title, QWidget *parent) :
    QGroupBox(title, parent)
{
    QGridLayout* layout = new QGridLayout();

    QColor colora(0,0,0,255);
    QColor colorb(0,200,0,255);

    for(int i=0; i<EegFrame::CONTACTS_NO; ++i){

        QVBoxLayout *labels = new QVBoxLayout();

        QLabel *labela = new QLabel(QString(EegFrame::contacts[i].c_str()));
        QPalette palette = labela->palette();
        palette.setColor(labela->foregroundRole(), colora);
        labela->setPalette(palette);
        QLabel *labelb = new QLabel(QString(EegFrame::contacts[i+1].c_str()));
        palette = labelb->palette();
        palette.setColor(labelb->foregroundRole(), colorb);
        labelb->setPalette(palette);
        labels->addWidget(labela);
        labels->addWidget(labelb);


        QCustomPlot *plot = new QCustomPlot(this);
        plot->setMinimumHeight(10);
//        plot->xAxis->setLabel("t");
//        plot->yAxis->setLabel("v");
        plot->xAxis->setVisible(false);
        plot->yAxis->setVisible(false);
        graphs[i]=plot->addGraph();
        graphs[++i]=plot->addGraph();
        plot->graph(0)->setPen(QPen(colora));
        plot->graph(1)->setPen(QPen(colorb));
        plot->setBackground(this->palette().color(QPalette::Background));

        layout->addLayout(labels, i+1, 0);
        layout->addWidget(plot, i+1, 1);
    }
    this->setLayout(layout);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);
}

void EegPlotWidget::eegFrame(QSharedPointer<EegFrame> framePtr){
    //instead of buffering

    static QList<double> x[EegFrame::CONTACTS_NO];
    static QList<double> y[EegFrame::CONTACTS_NO];
    static unsigned short counter = 0;

    counter=(counter+1)%32;

    QVector<QPair<double, double>> ranges;

    for(int e=0; e<EegFrame::CONTACTS_NO; ++e){

        x[e].push_back((double)framePtr->timestamp);
        y[e].push_back((double)framePtr->getValue(e));

        if(counter==31){
            this->graphs[e]->setData(x[e].toVector(), y[e].toVector());

            // set axes ranges, so we see all data:
            graphs[e]->parentPlot()->xAxis->setRange(x[e].at(0), x[e].at(x[e].size()-1));

            double minY = 40000;
            double maxY = 0;

            for(int index=0; index<y[e].length(); ++index){
                minY=qMin(minY, y[e][index]);
                maxY=qMax(maxY, y[e][index]);
            }

            ranges.append(QPair<double, double>(minY, maxY));

        }
        if(x[e].size()>=2000){
            x[e].pop_front();
            y[e].pop_front();
        }
    }


    for(int e=0; e<ranges.length()/2; ++e){
        double minY = qMin(ranges.at(2*e).first, ranges.at(2*e+1).first);
        double maxY = qMax(ranges.at(2*e).second, ranges.at(2*e+1).second);
//            qDebug()<<"Ranges length: "<<ranges.length()<<"maxY: "<<maxY<<", minY: "<<minY;
        double span = maxY-minY;
        graphs[2*e]->parentPlot()->yAxis->setRange(minY-0.1*span, maxY+0.1*span);
        graphs[2*e]->parentPlot()->replot();
    }
}
