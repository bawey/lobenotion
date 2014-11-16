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

    struct ClassifierDescriptor{
        QString subject;
        QString parameters;
        unsigned short charsCount;
        octave_value classifier;
    };

public:
    explicit ClassifiersModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:
    void slotTrainModel(QList<const P3SessionInfo*>);

private:
    QList<ClassifierDescriptor*> classifiers;
};

#endif // CLASSIFIERSMODEL_H
