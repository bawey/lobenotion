#include "sessionsmodel.h"
#include <octaveEmbedded/octaveproxy.h>
#include <master.h>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <spellerdumper.h>

SessionsModel::SessionsModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}


int SessionsModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid()){}
    return sessions.length();
}

int SessionsModel::columnCount(const QModelIndex &parent) const{
    if(parent.isValid()){}
    return 4;
}

QVariant SessionsModel::data(const QModelIndex &index, int role) const{
    if(role==Qt::DisplayRole){
        P3SessionInfo* record = sessions.at(index.row());
        switch(index.column()){
        case 0:
            return record->getCreated();
        case 1:
            return record->getSubjectName();
        case 2:
            return record->getPhrase();
        case 3:
            return QString(record->isClean()?"Fine":"Erratic");
        default:
            return QString("N/A");
        }
    }
    return QVariant();
}

QVariant SessionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Created");
            case 1:
                return QString("Subject name");
            case 2:
                return QString("Spelled phrase");
            case 3:
                return QString("Status");
            }
        }
    }
    return QVariant();
}

/**
 * @brief SessionsModel::slotLoadSession
 * @param nameroots absolute file paths without the trailing _summary/_data/_meta/_targets
 */
void SessionsModel::slotLoadSession(QStringList nameroots){

    OctaveProxy* proxy = Master::getInstance()->getOctaveProxy();
    for(int i=0; i<nameroots.length(); ++i){
        beginInsertRows(QModelIndex(), this->sessions.length(), this->sessions.length()+sessions.length()-1);

        QString subject;
        QString phrase;
        QDateTime created;
        bool clean = parseSummaryFile(nameroots.at(i)+"_summary", subject, phrase, created);

        P3SessionInfo* info = proxy->loadP3Session(nameroots.at(i));
        if(info!=NULL){
            P3SessionInfo* infoFull = new P3SessionInfo(subject, phrase, clean, created, info->getSession());
            this->sessions.push_back(infoFull);
        }else{
            qWarning()<<"Problems loading session "<<nameroots.at(i)<<". Ignoring and continuing.";
        }

        endInsertRows();
        qDebug()<<"Emitting dataChanged()";
        emit dataChanged(createIndex(0, 0), createIndex(sessions.length()-1, columnCount()-1));
    }
}

bool SessionsModel::parseSummaryFile(QString path, QString& subject, QString& phrase, QDateTime& created){
    QFile file(path);
    bool cleanSummary=true;
    if(file.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&file);
        QString line;
        while((line = in.readLine())!=NULL){
//          qDebug()<<"I read this: "<<line<<"from summary";
            QStringList tokens = line.split(':');
            if(tokens.length()==2){
                if(tokens.at(0).trimmed().toLower()=="subject"){
                    subject = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()=="phrase"){
                    phrase  = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()==SpellerDumper::SUMMARY_HEADER_ERROR){
                    cleanSummary=false;
                }
            }
        }
        QFileInfo finfo(file);
        created = finfo.created();
        file.close();
    }else{
        qWarning()<<"Failed to open the file: "<<path<<". PLS do sth!";
    }
    return cleanSummary;
}
