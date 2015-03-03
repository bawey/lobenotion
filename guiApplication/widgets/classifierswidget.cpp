#include "classifierswidget.h"
#include <QFile>
#include <QDir>
#include <settings.h>
#include <QDebug>
#include <QStringList>
#include <QDirIterator>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLayoutItem>
#include <settings.h>

ClassifiersWidget::ClassifiersWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(stackedLayout);
    stackedLayout->addWidget(errorPane);
    slotUpdateContent();
}

void ClassifiersWidget::slotUpdateContent(){

    QDir* d = new QDir(Settings::octaveScriptsRoot()+"/Classifiers");
    if(d->exists()){
        manageCheckboxConnections(false);
        boxes->clear();
        if(dataPane!=NULL){
            stackedLayout->removeWidget(dataPane);
            delete dataPane;
            dataPane = NULL;
        }

        dataPane = new QWidget(this);
        QFormLayout* formLayout = new QFormLayout();
        qDebug()<<"Classifiers directory exists";
        dataPane->setLayout(formLayout);

        QDirIterator dirs(d->absolutePath(), QDir::Dirs|QDir::NoDotAndDotDot ,QDirIterator::NoIteratorFlags);
        while(dirs.hasNext()){
            dirs.next();
            QString name = dirs.fileName();
            name = name.right(name.length()-1);
            qDebug() << name;
            Settings::addRuntimeClassifier(name);
            QCheckBox* cb = new QCheckBox();
            cb->setObjectName(name);
            if(Settings::classifierEnabled(name)){
                cb->setChecked(true);
            }
            QLabel* label = new QLabel(name);
            formLayout->addRow(label, cb);
            boxes->append(cb);
        }
        dataPane->setLayout(formLayout);
        stackedLayout->insertWidget(1, dataPane);
        stackedLayout->setCurrentIndex(1);
        manageCheckboxConnections(true);
    }else{
        stackedLayout->setCurrentIndex(0);
    }
}

void ClassifiersWidget::slotConfigurationChanged(QString parameter){
    if(parameter==Settings::OPT_OCTAVE_SCRIPTS_ROOT){
        this->slotUpdateContent();
    }
}

void ClassifiersWidget::slotClassifierEnabled(bool enabled){
    qDebug()<<"Classifier "<<sender()->objectName()<<" enabled: "<<enabled;
    emit signalClassifierEnabled(sender()->objectName(), enabled);
}

void ClassifiersWidget::manageCheckboxConnections(bool toConnect){
    for (int i=0; i<boxes->size(); ++i) {
        QCheckBox* box = boxes->at(i);
        if(toConnect){
            connect(box, SIGNAL(clicked(bool)), this, SLOT(slotClassifierEnabled(bool)));
        }else{
            disconnect(box, SIGNAL(clicked(bool)), this, SLOT(slotClassifierEnabled(bool)));
        }
    }
}
