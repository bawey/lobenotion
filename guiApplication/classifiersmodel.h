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


class ClassifiersModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    struct ClassifierDescriptor{
        QString subject;
        QString parameters;
        unsigned short charsCount;
        octave_value classifier;
        octave_value tdMean;
        octave_value tdStd;
    };

public:
    explicit ClassifiersModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

    void signalCurrentClassifierChanged(int);
public slots:
    void slotTrainModel(QSharedPointer<QList<const P3SessionInfo *> > sharedInfos);
    void slotTestModel(unsigned short index, QSharedPointer<QList<const P3SessionInfo *> > testData);
    void slotSetCurrentClassifier(int);


private:
    QList<ClassifierDescriptor*> classifiers;
    int currentClassifier=0;
};

#endif // CLASSIFIERSMODEL_H
