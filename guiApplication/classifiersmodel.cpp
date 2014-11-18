#include "classifiersmodel.h"
#include <QDebug>
#include <octaveEmbedded/octaveproxy.h>
#include <master.h>
#include <octave/toplev.h>
#include <octave/octave.h>
#include <octave/oct.h>
#include <QModelIndex>

ClassifiersModel::ClassifiersModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

/**
 * @achtung deletion will be performed on the argument
 * @brief ClassifiersModel::slotTrainModel
 * @param infos
 */
void ClassifiersModel::slotTrainModel(QSharedPointer<QList<const P3SessionInfo*>> sharedInfos){
    OctaveProxy* proxy = Master::getInstance()->getOctaveProxy();

    ClassifierDescriptor* desc = new ClassifierDescriptor();
    octave_value classifier = proxy->pickBestModel(*sharedInfos, desc);


    QString subjects = sharedInfos->at(0)->getSubjectName();

    unsigned short charsCount = 0;

    foreach (const P3SessionInfo* info, *sharedInfos) {
        charsCount+=info->getPhrase().length();
        if(!subjects.contains(info->getSubjectName().trimmed())){
            if(subjects.length()>=0){
                subjects.append(", ");
            }
//            qDebug()<<"subjects was: "<<subjects;
            subjects.append(info->getSubjectName().trimmed());
//            qDebug()<<"subjects is: "<<subjects;
//            qDebug()<<"info->subjectName is: "<<info->getSubjectName().trimmed();
        }
    }

    desc->charsCount=charsCount;
    desc->subject=subjects;

    beginInsertRows(QModelIndex(), classifiers.length(), classifiers.length());
    this->classifiers.append(desc);
    endInsertRows();
}

void ClassifiersModel::slotTestModel(unsigned short index, QSharedPointer<QList<const P3SessionInfo *> > testSessionInfos){
    ClassifierDescriptor *desc = classifiers.at(index);
    OctaveProxy* proxy = Master::getInstance()->getOctaveProxy();
    proxy->askClassifier(desc, *testSessionInfos);
}

int ClassifiersModel::rowCount(const QModelIndex & parent) const{
    if(parent.isValid()){}
    return classifiers.length();
}
int ClassifiersModel::columnCount(const QModelIndex &parent) const{
    if(parent.isValid()){}
    return 3;
}
QVariant ClassifiersModel::data(const QModelIndex &index, int role) const {
    if(role==Qt::DisplayRole){
        ClassifierDescriptor* record = classifiers.at(index.row());
            switch(index.column()){
            case 0:
                return record->subject;
            case 1:
                return record->charsCount;
            case 2:
                return record->parameters;
            default:
                return QString("N/A");
            }
        }
        return QVariant();
    return QVariant();
}

QVariant ClassifiersModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QString("Subject");
                case 1:
                    return QString("Chars used");
                case 2:
                    return QString("Classifier details");
                default:
                    return QString("N/A");
            }
        }
    }
    return QVariant();
}

void ClassifiersModel::slotSetCurrentClassifier(int classifierToBe){
    if(classifierToBe<this->classifiers.length()){
        qDebug()<<"changing current classifier from: "<<this->currentClassifier
                  <<" to: "<<classifierToBe;
        this->currentClassifier=classifierToBe;
        emit signalCurrentClassifierChanged(currentClassifier);
    }else{
        qWarning()<<"Attempted to set the current classifier as "<<classifierToBe<<" with only "
                 <<classifiers.length()<<" available!";
    }
}
