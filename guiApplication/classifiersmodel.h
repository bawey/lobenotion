#ifndef CLASSIFIERSMODEL_H
#define CLASSIFIERSMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <octaveEmbedded/p3sessioninfo.h>
#include <QString>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <QSharedPointer>
#include <QVector>
#include <classifierinfo.h>


class ClassifiersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ClassifiersModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    ClassifierInfo* chosenClassifier(){
        return currentClassifier > -1 ? classifiers.at(currentClassifier) : NULL;
    }



signals:

    void signalCurrentClassifierChanged(int);
    void signalSymbolClassified(int row, int column, float confidence);
public slots:
    void slotTrainModel(QSharedPointer<QList<const P3SessionInfo *> > sharedInfos);
    void slotTestModel(unsigned short index, QSharedPointer<QList<const P3SessionInfo *> > testData);
    void slotSetCurrentClassifier(int);
    void slotAskCurrentClassifier(QSharedPointer<QVector<int>> data, QSharedPointer<QVector<int>> meta,
                                  QSharedPointer<QVector<int>> trg);

private:
    QList<ClassifierInfo*> classifiers;
    int currentClassifier=-1;
    void onlineClasificationJob(QSharedPointer<QVector<int> > data, QSharedPointer<QVector<int> > meta, QSharedPointer<QVector<int> > trg);
};

#endif // CLASSIFIERSMODEL_H
