#include "spellerwidget.h"
#include <QGridLayout>
#include <QMainWindow>
#include <QDebug>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QStackedLayout>

QString SpellerWidget::styleRegular=QString("background-color : black; color: white;");
QString SpellerWidget::styleHighlight=QString("background-color : green; color: white;");

SpellerWidget::SpellerWidget()
{
    highlighted=0;
    stackedLayout=new QStackedLayout();
    message=new QLabel("P300 speller");
    message->setFont(QFont("Arial", 40));
    message->setStyleSheet(styleRegular);
    message->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    stackedLayout->addWidget(message);

    QWidget* keyboard = new QWidget();
    QGridLayout* keyboardLayout = new QGridLayout();
    keyboardLayout->setHorizontalSpacing(0);
    keyboardLayout->setVerticalSpacing(0);
    keyboardLayout->setContentsMargins(0,0,0,0);
    keyboard->setLayout(keyboardLayout);

    for(int r=0; r<MATRIX_DIM; ++r){

        for(int c=0; c<MATRIX_DIM; c++){
            int index = r*MATRIX_DIM+c;
            QLabel* label = new QLabel(QChar((char)(index<26?65:22)+index));
            QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            label->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
            label->setSizePolicy(sizePolicy);
            label->setMinimumHeight(120);
            label->setContentsMargins(0,0,0,0);
            label->setMinimumWidth(120);
            label->setStyleSheet("background-color: black;  color: white;");
            QFont f( "Arial", 20);
            label->setFont( f);
            keyboardLayout->addWidget(label, r, c, 1, 1, Qt::AlignCenter|Qt::AlignHCenter);

            tiles[r*MATRIX_DIM+c]=label;
        }
    }

    stackedLayout->addWidget(keyboard);
    stackedLayout->setCurrentIndex(1);
    this->setLayout(stackedLayout);
    this->setWindowTitle("P300 Speller");
    this->show();
    keyboard->setFixedSize(keyboard->size());
}

void SpellerWidget::resizeEvent(QResizeEvent* event){
     QWidget::resizeEvent(event);
    int height=event->size().height();
    int width=event->size().width();

    int h = (int)(height/MATRIX_DIM);
    int w = (int)(width/MATRIX_DIM);
    for(int i=0; i<MATRIX_DIM*MATRIX_DIM; ++i){
//        tiles[i]->setMinimumWidth(w);
//        tiles[i]->setMinimumHeight(h);
//        tiles[i]->setMaximumWidth(w);
//        tiles[i]->setMaximumHeight(h);
    }

    this->layout()->setContentsMargins(0,0,0,0);
    event->accept();

    qDebug()<<"dims: "<<QString::number(width)<<", "<<QString::number(height);
}


void SpellerWidget::highlightRow(int number){
    stackedLayout->setCurrentIndex(1);
    int rowNo = number%MATRIX_DIM;
    for(int i=0; i<MATRIX_DIM; ++i){
        tiles[rowNo*MATRIX_DIM+i]->setStyleSheet(styleHighlight);
    }
    highlighted=1+rowNo;
}


void SpellerWidget::highlightColumn(int number){
    stackedLayout->setCurrentIndex(1);
    int colNo = number%MATRIX_DIM;
    for(int i=0; i<MATRIX_DIM; ++i){
        tiles[i*MATRIX_DIM+colNo]->setStyleSheet(styleHighlight);
    }
    highlighted=-1*colNo-1;
}


void SpellerWidget::unhighlight(){
    stackedLayout->setCurrentIndex(1);
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

void SpellerWidget::displayInstruction(QString messageString){
    stackedLayout->setCurrentIndex(0);
    this->message->setText(messageString);
}

void SpellerWidget::highlightTile(int number){


//    for(int i=0; i<MATRIX_DIM*MATRIX_DIM; ++i){
//        if(i==0){

//        }else{
//            tiles[i]->setVisible(false);
//        }
//    }
}

void SpellerWidget::spellerMessage(QString str){
    stackedLayout->setCurrentIndex(0);
    message->setText(str);
}

void SpellerWidget::randomHint(){
    QString content = tiles[qrand()%(MATRIX_DIM*MATRIX_DIM)]->text();
    stackedLayout->setCurrentIndex(0);
    message->setText(
        QString("count highlights of <font style='bold' color='green'>")+content
                +QString("</font>"));
}

void SpellerWidget::eegFrameNotification(){
    static int frameCount;

}
