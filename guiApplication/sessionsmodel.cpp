#include "sessionsmodel.h"
#include <octaveEmbedded/octaveproxy.h>
#include <master.h>
#include <QList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

SessionsModel::SessionsModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}


int SessionsModel::rowCount(const QModelIndex &parent) const{
//    qDebug()<<"reporting "<<sessions.length()<<" rows in the model";
    return sessions.length();
}

int SessionsModel::columnCount(const QModelIndex &parent) const{
    return 4;
}

QVariant SessionsModel::data(const QModelIndex &index, int role) const{
//    qDebug()<<"about to divulge data model's secrets";
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
//    qDebug()<<"about to inform on header's structure";
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

void SessionsModel::slotLoadSession(QString dirpath, QString subject, unsigned short number){
    OctaveProxy* proctave = Master::getInstance()->getOctaveProxy();
    QList<unsigned short> sessionNos;
    sessionNos.append(number);
    octave_value octave_session = proctave->loadMergeAndDownsample(dirpath, subject, sessionNos);

    //this->sessions.append(new P3SessionInfo(subject, "unknown", false, octave_session));
    emit dataChanged(createIndex(sessions.length()-1, 0), createIndex(sessions.length()-1, columnCount()));
}

void SessionsModel::slotLoadSession(QString dirpath, QStringList nameroots){
    octave_value_list sessions = Master::getInstance()->getOctaveProxy()->loadSessions(dirpath, nameroots);
    for(int i=0; i<sessions.length(); ++i){
        //P3SessionInfo* info = new P3SessionInfo("Dummy subject", "Phrase?", false, sessions(i));
        //this->sessions.push_back(info);
    }
    emit dataChanged(createIndex(0, 0), createIndex(sessions.length()-1, columnCount()));
}

void SessionsModel::slotLoadSession(QStringList nameroots){
    octave_value_list sessions = Master::getInstance()->getOctaveProxy()->loadSessions(nameroots);
    beginInsertRows(QModelIndex(), this->sessions.length(), this->sessions.length()+sessions.length()-1);
    if(nameroots.length()!=sessions.length()){
        qDebug()<<"some session was not loaded correctly. Need to handle that";

    }else{
        for(int i=0; i<sessions.length(); ++i){
            QString subject;
            QString phrase;
            QDateTime created;
            bool clean = parseSummaryFile(nameroots.at(i)+"_summary", subject, phrase, created);

            qDebug()<<"Pushing onto sessions list";
            P3SessionInfo* info = new P3SessionInfo(subject, phrase, clean, created, sessions(i));
            this->sessions.push_back(info);
        }
    }
    endInsertRows();
    qDebug()<<"Emitting dataChanged()";
    emit dataChanged(createIndex(0, 0), createIndex(sessions.length()-1, columnCount()-1));
}

bool SessionsModel::parseSummaryFile(QString path, QString& subject, QString& phrase, QDateTime& created){
    QFile file(path);
    bool cleanSummary=true;
    if(file.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&file);
        QString line;
        while((line = in.readLine())!=NULL){
            qDebug()<<"I read this: "<<line<<"from summary";
            QStringList tokens = line.split(':');
            if(tokens.length()==2){
                if(tokens.at(0).trimmed().toLower()=="subject"){
                    subject = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()=="phrase"){
                    phrase  = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()=="error"){
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
