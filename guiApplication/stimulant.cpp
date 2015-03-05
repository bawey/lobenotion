#include "stimulant.h"
#include <QObject>
#include <QThread>
#include <QGridLayout>
#include <QString>
#include <Qt>
#include <QFont>
#include <QApplication>
#include <QStyle>
#include <QSizePolicy>
#include <QChar>


Stimulant::Stimulant(EegDaq* daqModule, bool trainingOnly) : daq(daqModule), trainingMode(trainingOnly)
{


//stimulant no longer controls speller actions. speller is autonomous now
//   QObject::connect(this, SIGNAL(spellerHighlightColumn(int)), &speller, SLOT(highlightColumn(int)));
//   QObject::connect(this, SIGNAL(spellerHighlightRow(int)), &speller, SLOT(highlightRow(int)));
//   QObject::connect(this, SIGNAL(spellerUnhighlight()), &speller, SLOT(unhighlight()));
//   QObject::connect(this, SIGNAL(spellerRandomHint()), &speller, SLOT(randomHint()));
//    QObject::connect(daq, SIGNAL(eegFrame(QSharedPointer<EegFrame>)), &speller, SLOT(eegFrameNotification()));

}

void Stimulant::run(){
//    bool column=true;
//    while(true){
//        if(trainingMode){
//            emit spellerRandomHint();
//            msleep(HIGHLIGHT_DURATION*3);
//            for(int i=0; i<10; ++i){
//                int tileNo = qrand();
//                emit spellerUnhighlight();
//                msleep(INTERVAL_DURATION);
//                if(column){
//                    emit spellerHighlightColumn(tileNo);
//                }else{
//                    emit spellerHighlightRow(tileNo);
//                }
//                msleep(HIGHLIGHT_DURATION);
//                column=!column;
//            }
//        }else{
//            msleep(INTERVAL_DURATION);
//            int rand = qrand();
//            if(column){
//                emit spellerHighlightColumn(rand);
//            }else{
//                emit spellerHighlightRow(rand);
//            }
//            msleep(HIGHLIGHT_DURATION);
//            emit spellerUnhighlight();
//            column=!column;
//        }
//    }
}

