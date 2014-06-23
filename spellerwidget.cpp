#include "spellerwidget.h"
#include <QGridLayout>
#include <QMainWindow>
#include <QDebug>

QString SpellerWidget::styleRegular=QString("background-color : black; color: white;");
QString SpellerWidget::styleHighlight=QString("background-color : green; color: white;");

SpellerWidget::SpellerWidget()
{
    QGridLayout* layout = new QGridLayout();
    this->setLayout(layout);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);


    for(int r=0; r<MATRIX_DIM; ++r){
        layout->setColumnStretch(r,99);
        layout->setRowStretch(r,99);
        for(int c=0; c<MATRIX_DIM; c++){
            int index = r*MATRIX_DIM+c;
            QLabel* label = new QLabel(QChar((char)(index<26?65:22)+index));
            label->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
            label->setScaledContents(false);
            label->setAutoFillBackground(true);
            label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            label->setMinimumHeight(100);
            label->setMinimumWidth(100);
            label->setSizeIncrement(1,1);
            label->setMaximumHeight(10000);
            label->setMaximumWidth(10000);
            label->setStyleSheet("background-color: black;  color: white;");
            label->setMargin(0);
            layout->addWidget(label, r, c, 1, 1, Qt::AlignCenter|Qt::AlignHCenter);
            QFont f( "Arial", 20);
            label->setFont( f);

            tiles[r*MATRIX_DIM+c]=label;
        }
    }

    this->resize(640, 480);
    this->setWindowTitle("P300 Speller");
    this->show();

}

void SpellerWidget::resizeEvent(QResizeEvent* event){
     QWidget::resizeEvent(event);
    int height=this->frameSize().height();
    int width=this->frameSize().width();

    int h = (int)((double)height/MATRIX_DIM);
    int w = (int)((double)width/MATRIX_DIM);
    for(int i=0; i<MATRIX_DIM*MATRIX_DIM; ++i){
        //tiles[i]->setMinimumWidth(w);
        //tiles[i]->setMinimumHeight(h);
        //tiles[i]->setMaximumWidth(w);
        //tiles[i]->setMaximumHeight(h);
    }

    qDebug()<<"dims: "<<QString::number(width)<<", "<<QString::number(height);
}


void SpellerWidget::highlightRow(int number){
    int rowNo = number%MATRIX_DIM;
    for(int i=0; i<MATRIX_DIM; ++i){
        tiles[rowNo*MATRIX_DIM+i]->setStyleSheet(styleHighlight);
    }
    highlighted=1+rowNo;
}


void SpellerWidget::highlightColumn(int number){
    int colNo = number%MATRIX_DIM;
    for(int i=0; i<MATRIX_DIM; ++i){
        tiles[i*MATRIX_DIM+colNo]->setStyleSheet(styleHighlight);
    }
    highlighted=-1*colNo-1;
}


void SpellerWidget::unhighlight(){
    if(highlighted>0){
        int rowNo = highlighted-1;
        for(int i=0; i<MATRIX_DIM; ++i){
            tiles[rowNo*MATRIX_DIM+i]->setStyleSheet(styleRegular);
        }
    }else if(highlighted<0){
        int colNo = -1*highlighted-1;
        for(int i=0; i<MATRIX_DIM; ++i){
            tiles[i*MATRIX_DIM+colNo]->setStyleSheet(styleRegular);
        }
    }
    highlighted=0;
}

