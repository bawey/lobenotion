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


Stimulant::Stimulant()
{

   speller.show();

   QObject::connect(this, SIGNAL(orderHighlightColumn(int)), &speller, SLOT(highlightColumn(int)));
   QObject::connect(this, SIGNAL(orderHighlightRow(int)), &speller, SLOT(highlightRow(int)));
   QObject::connect(this, SIGNAL(orderUnhighlight()), &speller, SLOT(unhighlight()));

}


void Stimulant::run(){
    bool column;

    while(true){
        msleep(INTERVAL_DURATION);
        int rand = qrand();
        if(column){
            emit orderHighlightColumn(rand);
        }else{
            emit orderHighlightRow(rand);
        }
        msleep(HIGHLIGHT_DURATION);
        emit orderUnhighlight();
        column=!column;
    }
}



