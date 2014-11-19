#ifndef SESSIONSMODEL_H
#define SESSIONSMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <octaveEmbedded/p3sessioninfo.h>
#include <QDateTime>

class SessionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SessionsModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;


    QList<const P3SessionInfo*>* getSessionsAt(QList<unsigned short> positions) const {
        QList<const P3SessionInfo* >* list = new QList<const P3SessionInfo*>();
        foreach (unsigned short pos, positions) {
            list->append(sessions.at(pos));
        }
        return list;
    }

signals:



public slots:
    void slotLoadSession(QStringList nameroots);
private:
    bool parseSummaryFile(QString path, QString& subject, QString& phrase, QDateTime& created);
    QVector<P3SessionInfo*> sessions;
};

#endif // SESSIONSMODEL_H
