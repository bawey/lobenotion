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
//    int width=event->size().width();

    int h = (int)(height/MATRIX_DIM);
//    int w = (int)(width/MATRIX_DIM);

    QFont font = tiles[0]->font();
//    font.setPixelSize(font.pixelSize()*hStretch);
    font.setPixelSize(h*0.66);

    for(int i=0; i<MATRIX_DIM*MATRIX_DIM; ++i){
          tiles[i]->setFont(font);
    }

    this->layout()->setContentsMargins(0,0,0,0);
    event->accept();
    //qDebug()<<"dims: "<<QString::number(width)<<", "<<QString::number(height);
}


void SpellerWidget::highlightRow(int rowNo){
    stackedLayout->setCurrentIndex(1);
    for(int i=0; i<MATRIX_DIM; ++i){
        tiles[rowNo*MATRIX_DIM+i]->setStyleSheet(SpellerWidget::getCellHighlightedStyle());
    }
    long time = Timer::getTime();
    highlighted=1+rowNo;
    emit highlight(highlighted, time);
}


void SpellerWidget::highlightColumn(int colNo){
//    qDebug()<<"highlite col: "<<colNo<<"at: "<<Timer::getTime();
    stackedLayout->setCurrentIndex(1);
    for(int i=0; i<MATRIX_DIM; ++i){
        tiles[i*MATRIX_DIM+colNo]->setStyleSheet(SpellerWidget::getCellHighlightedStyle());
    }
    highlighted=-1*colNo-1;
    long time = Timer::getTime();
    emit highlight(highlighted, time);
}


void SpellerWidget::unhighlight(){
    stackedLayout->setCurrentIndex(1);
    if(highlighted>0){
        int rowNo = highlighted-1;
        for(int i=0; i<MATRIX_DIM; ++i){
            tiles[rowNo*MATRIX_DIM+i]->setStyleSheet(SpellerWidget::getCellRegularStyle());
        }
    }else if(highlighted<0){
        int colNo = -1*highlighted-1;
        for(int i=0; i<MATRIX_DIM; ++i){
            tiles[i*MATRIX_DIM+colNo]->setStyleSheet(SpellerWidget::getCellRegularStyle());
        }
    }
    highlighted=0;
}

void SpellerWidget::displayInstruction(QString messageString){
    stackedLayout->setCurrentIndex(0);
    this->message->setText(messageString);
}

//assuming values 1-6 for clear communication between classes
void SpellerWidget::highlightTile(int row, int column){
    --row;
    --column;
    tiles[row*MATRIX_DIM+column]->setStyleSheet(SpellerWidget::getCellHighlightedStyle());
    this->highlighted=row;
}

void SpellerWidget::spellerMessage(QString str){
    stackedLayout->setCurrentIndex(0);
    message->setText(str);
}

/**
 * Now it will only get a random number (this feature needs to be moved outside the widget!)
 * @brief SpellerWidget::randomHint
 */
void SpellerWidget::randomHint(){
    //this should be a value from the range 0-35 for BCI Speller
    int index = (int)((qrand()/(double)RAND_MAX*(MATRIX_DIM*MATRIX_DIM-1))+0.5);
//    QString content = tiles[index]->text();
//    stackedLayout->setCurrentIndex(0);
//    message->setText(
//        QString("count highlights of <font style='bold' color='green'>")+content
//                +QString("</font>"));

    //this should be propagated by the means of slots and signals
    //the values should be from range 1-6
    int row = index/MATRIX_DIM + 1;
    int column = index%MATRIX_DIM + 1;

    highlightTile(row, column);
    emit hint(row, column);
}

/**
 * @brief SpellerWidget::eegFrameNotification is synchronized with DAQ to display particular stimuli
 */
void SpellerWidget::eegFrameNotification(){ //training cycle: [hint->interval->(highlight->interval)]
    frameCount=frameCount%epochFrames;
    if(frameCount==0){
        randomHint();
    }else if(frameCount==hintFrames){
        unhighlight();
    }else if(frameCount>=hintFrames+3*intervalFrames){
        int mod = (frameCount-3*intervalFrames-hintFrames)%(exposureFrames+intervalFrames);
        if(mod==0){
            int randIndex = (qrand()/(double)RAND_MAX*MATRIX_DIM);
//            qDebug()<<"Rand index: "<<randIndex;
            if(nextHighlightColumn){
                highlightColumn(randIndex);
            }else{
                highlightRow(randIndex);
            }
            nextHighlightColumn=!nextHighlightColumn;
        }else if(mod==exposureFrames){
            unhighlight();
        }
    }
    ++frameCount;
}

void SpellerWidget::trainingResetFramesCount(){
    frameCount=0;
    nextHighlightColumn=0;
}

QString SpellerWidget::getCellRegularStyle(){
    return QString("background-color: black; color: gray; font-weight: normal;");
}

QString SpellerWidget::getCellHighlightedStyle(){
    return QString("background-color: black; color: white; font-weight: bold;");
}
