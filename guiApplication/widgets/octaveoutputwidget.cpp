#include "octaveoutputwidget.h"
#include <QHBoxLayout>

OctaveOutputWidget::OctaveOutputWidget(QWidget *parent) :
    QGroupBox("Octave output", parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(&textEdit);
    this->setLayout(layout);
}

void OctaveOutputWidget::slotOctaveOutput(QString output){
    textEdit.insertPlainText(output);
}
