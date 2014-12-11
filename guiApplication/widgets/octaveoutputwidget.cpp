#include "octaveoutputwidget.h"
#include <QHBoxLayout>
#include <QScrollBar>
#include <QDebug>

OctaveOutputWidget::OctaveOutputWidget(QWidget *parent) :
    QGroupBox("Octave output", parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(&textEdit);
    textEdit.setReadOnly(true);
    textEdit.setMaximumBlockCount(200);
    this->setLayout(layout);

    connect(textEdit.verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotVScrollBarChanged(int)));
}

void OctaveOutputWidget::slotOctaveOutput(QString output){
    textEdit.moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    textEdit.insertPlainText(output);
    if(sliderAllDown){
        textEdit.verticalScrollBar()->setValue(textEdit.verticalScrollBar()->maximum());
    }
}

void OctaveOutputWidget::slotVScrollBarChanged(int position){
    this->sliderAllDown = (position==textEdit.verticalScrollBar()->maximum());
    //qDebug()<<"slider all the way down: "<<sliderAllDown;
}
