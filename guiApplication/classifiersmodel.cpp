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
void ClassifiersModel::slotTrainModel(QList<const P3SessionInfo*> infos, bool deleteInfos){
    qDebug()<<"signal received to train a model... panicking!";

    OctaveProxy* proxy = Master::getInstance()->getOctaveProxy();
    //1: need to merge the sessions into one!
    //2: need to launch training procedure for that session
    //3: need to store the best classfier
    octave_value classifier = proxy->pickBestModel(infos);
    //4: would be not bad to retrieve classifiers properties and store them in this->classifiers
    ClassifierDescriptor* desc = new ClassifierDescriptor();

    QString subjects = "";
    QString paramsters="undetermined";
    unsigned short charsCount = 0;

    foreach (const P3SessionInfo* info, infos) {
        charsCount+=info->getPhrase().length();
        if(!subjects.contains(info->getSubjectName())){
            if(subjects.length()>=0){
                subjects.append(', ');
            }
            subjects.append(info->getSubjectName());
        }
    }

    desc->charsCount=charsCount;
    desc->parameters=paramsters;
    desc->subject=subjects;
    desc->classifier=classifier;

    beginInsertRows(QModelIndex(), classifiers.length(), classifiers.length());
    this->classifiers.append(desc);
    endInsertRows();
    if(deleteInfos){
        delete infos;
    }
}

int ClassifiersModel::rowCount(const QModelIndex & parent) const{
    return classifiers.length();
}
int ClassifiersModel::columnCount(const QModelIndex &parent) const{
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
