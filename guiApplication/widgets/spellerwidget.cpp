#include "spellerwidget.h"
#include <QGridLayout>
#include <QMainWindow>
#include <QDebug>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <timer.h>

SpellerWidget::SpellerWidget()
{
    highlighted=0;
    stackedLayout=new QStackedLayout();
    message=new QLabel("P300 speller");
    message->setFont(QFont("Arial", 40));
    message->setStyleSheet(SpellerWidget::getCellRegularStyle());
    message->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    stackedLayout->addWidget(message);

    QWidget* keyboard = new QWidget();
    keyboard->setStyleSheet("background-color: black;");
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
            label->setStyleSheet(SpellerWidget::getCellRegularStyle());
            QFont f( "Arial", 20);
            label->setFont( f);
            keyboardLayout->addWidget(label, r, c, 1, 1, Qt::AlignCenter|Qt::AlignHCenter);

            tiles[r*MATRIX_DIM+c]=label;
        }
    }

    stackedLayout->addWidget(keyboard);
    stackedLayout->setCurrentIndex(1);
    this->setLayout(stackedLayout);
    //can be enabled for DEBUG
    //this->setStyleSheet("background-color: pink;");
    //keyboard->setFixedSize(keyboard->size());
}


void SpellerWidget::resizeEvent(QResizeEvent* event){
    QWidget::resizeEvent(event);

    int height=event->size().height();
    // int width=event->size().width();

    int h = (int)(height/MATRIX_DIM);
    // int w = (int)(width/MATRIX_DIM);

    QFont font = tiles[0]->font();
    //  font.setPixelSize(font.pixelSize()*hStretch);
    font.setPixelSize(h*0.66);

    for(int i=0; i<MATRIX_DIM*MATRIX_DIM; ++i){
          tiles[i]->setFont(font);
    }

    this->layout()->setContentsMargins(0,0,0,0);
    event->accept();
    //qDebug()<<"dims: "<<QString::number(width)<<", "<<QString::number(height);
}


void SpellerWidget::highlight(short code){
    this->highlightOnOff(code, true);
}

void SpellerWidget::unhighlight(){
    this->highlightOnOff(this->highlighted, false);
}

void SpellerWidget::highlightOnOff(short code, bool onOff){
    stackedLayout->setCurrentIndex(1);
    qDebug()<<(onOff?"highlighting: ":"dimming: ");
    if(code<0){
        for(int i=0; i<MATRIX_DIM; ++i){
//            qDebug()<<"tile: "<<(i*MATRIX_DIM+(qAbs(code)-1));
            tiles[i*MATRIX_DIM+(qAbs(code)-1)]->setStyleSheet(onOff ? SpellerWidget::getCellHighlightedStyle() : SpellerWidget::getCellRegularStyle());
        }
    }else if(code>0){
        for(int i=0; i<MATRIX_DIM; ++i){
//            qDebug()<<"tile: "<<((code-1)*MATRIX_DIM+i);
            tiles[(code-1)*MATRIX_DIM+i]->setStyleSheet(onOff ? SpellerWidget::getCellHighlightedStyle() : SpellerWidget::getCellRegularStyle());
        }
    }
    highlighted= onOff ? code : 0;
}

//assuming values 1-6 for clear communication between classes
void SpellerWidget::highlightTile(short row, short column){
    tiles[(row-1)*MATRIX_DIM+(column-1)]->setStyleSheet(SpellerWidget::getCellHighlightedStyle());
    // could also be set to column. all the tiles along this line will later be reset
    this->highlighted=row;
}

void SpellerWidget::showMessage(QString str){
    stackedLayout->setCurrentIndex(0);
    message->setText(str);
}

QString SpellerWidget::getCellRegularStyle(){
    return QString("background-color: black; color: gray; font-weight: normal;");
}

QString SpellerWidget::getCellHighlightedStyle(){
    return QString("background-color: black; color: white; font-weight: bold;");
}
