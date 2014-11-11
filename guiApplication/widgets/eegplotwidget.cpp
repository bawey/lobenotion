#include "eegplotwidget.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QGridLayout>

EegPlotWidget::EegPlotWidget(QString title, QWidget *parent) :
    QGroupBox(title, parent)
{
    QGridLayout* layout = new QGridLayout();
    for(int i=0; i<EegFrame::CONTACTS_NO; ++i){
        QLabel *label = new QLabel(QString(EegFrame::contacts[i].c_str())+" "+
            QString(EegFrame::contacts[i+1].c_str()));

        QCustomPlot *plot = new QCustomPlot();
//        plot->xAxis->setLabel("t");
//        plot->yAxis->setLabel("v");
        plot->xAxis->setVisible(false);
        plot->yAxis->setVisible(false);
        //plot->yAxis->setRange(8000, 10000);
        graphs[i]=plot->addGraph();
        graphs[++i]=plot->addGraph();
        plot->graph(1)->setPen(QPen(QColor(255,0,0,255)));

        layout->addWidget(label, i+1, 0);
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

            graphs[e]->parentPlot()->yAxis->setRange(minY, maxY);
            graphs[e]->parentPlot()->replot();

        }
        if(x[e].size()>=2000){
            x[e].pop_front();
            y[e].pop_front();
        }
    }
}
