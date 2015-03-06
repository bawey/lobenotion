#include "classifiersmodel.h"
#include <QDebug>
#include <octaveproxy.h>
#include <ClassifierOutput.h>
#include <master.h>
#include <QModelIndex>
#include <classifierinfo.h>
#include <QtConcurrent/QtConcurrent>

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

    ClassifierInfo* desc = proxy->pickBestModel(*sharedInfos);
    if(desc==NULL){
        qWarning()<<"pickBestModel returned a null ClassifierInfo pointer";
        return;
    }
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
    desc->dataRepeats=sharedInfos->at(0)->getRepeats();
    desc->dataHighStint=sharedInfos->at(0)->getHighlightStint();
    desc->dataDimStint=sharedInfos->at(0)->getDimStint();

    beginInsertRows(QModelIndex(), classifiers.length(), classifiers.length());
    this->classifiers.append(desc);
    endInsertRows();
}

void ClassifiersModel::testModelJob(unsigned short index, QSharedPointer<QList<const P3SessionInfo *> > testSessionInfos){
    ClassifierInfo *desc = classifiers.at(index);
    OctaveProxy* proxy = Master::getInstance()->getOctaveProxy();
    proxy->askClassifier(desc, &*testSessionInfos);
}

void ClassifiersModel::slotTestModel(unsigned short index, QSharedPointer<QList<const P3SessionInfo *> > testSessionInfos){
    QtConcurrent::run(this, &ClassifiersModel::testModelJob, index, testSessionInfos);
}

void ClassifiersModel::slotAnalyzeConfidence( QSharedPointer<QList<const P3SessionInfo *>> data) const {
    OctaveProxy* proxy = Master::getInstance()->getOctaveProxy();
    proxy->analyzeConfidence(chosenClassifier(), data);
}

int ClassifiersModel::rowCount(const QModelIndex & parent) const{
    if(parent.isValid()){}
    return classifiers.length();
}
int ClassifiersModel::columnCount(const QModelIndex &parent) const{
    if(parent.isValid()){}
    return 6;
}
QVariant ClassifiersModel::data(const QModelIndex &index, int role) const {
    if(role==Qt::DisplayRole){
        ClassifierInfo* record = classifiers.at(index.row());
            switch(index.column()){
            case 0:
                return record->subject;
            case 1:
                return record->charsCount;
            case 2:
                return record->dataRepeats;
            case 3:
                return record->dataHighStint;
            case 4:
                return record->dataDimStint;
            case 5:
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
                    return QString("Chars");
                case 2:
                    return "Repeats";
                case 3:
                    return "HiLite";
                case 4:
                    return "Dim";
                case 5:
                    return QString("Classifier");
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

void ClassifiersModel::onlineClasificationJob(QSharedPointer<QVector<int> > data, QSharedPointer<QVector<int> > meta, QSharedPointer<QVector<int> > trg){
    if(currentClassifier>=0 && classifiers.length()>currentClassifier){
        int timestart = Timer::getTime();
        P3SessionInfo* info = Master::getInstance()->getOctaveProxy()->toP3Session(data, meta, trg);
        if(info==NULL){
            qWarning()<<"Null P3Session info returned by toP3Session";
        } else {
            qDebug()<<"Converting to P3Session took "<<(Timer::getTime()-timestart)<<" ms";
            QSharedPointer<QVector<ClassifierOutput*>> results = Master::getInstance()->getOctaveProxy()->askClassifier(classifiers.at(currentClassifier), info);
            QString output = "Results: ";
            for(int i=0; i<results->length(); ++i){
                ClassifierOutput* co = results->at(i);
                output.append(QString::number(co->row)).append(" x ").append(QString::number(co->column)).append("[ ")
                        .append(QString::number(co->confidence)).append("] ");
                //Well, this should depend on: confidence, epochs elapsed...
                emit signalSymbolClassified(qAbs(co->row), qAbs(co->column), co->confidence);
            }
            qDebug()<<"And jointly with clasification: "<<(Timer::getTime()-timestart)<<"ms. "<<output;

            delete info;
        }
    } else {
        qWarning()<<"Received a request to classify sth, but current classifier is "<<currentClassifier
                    <<" and there are "<<classifiers.length()<<" classfiers";
    }
}

void ClassifiersModel::slotAskCurrentClassifier(QSharedPointer<QVector<int> > data, QSharedPointer<QVector<int> > meta, QSharedPointer<QVector<int> > trg){
    QtConcurrent::run(this, &ClassifiersModel::onlineClasificationJob, data, meta, trg);
}
