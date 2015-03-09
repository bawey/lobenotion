#include "sessionsmodel.h"
#include <octaveproxy.h>
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
    qRegisterMetaType<QVector<int>>("QVector<int>");
}


int SessionsModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid()){}
    return sessions.length();
}

int SessionsModel::columnCount(const QModelIndex &parent) const{
    if(parent.isValid()){}
    return 7;
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
        case 4:
            return record->getRepeats();
        case 5:
            return record->getHighlightStint();
        case 6:
            return record->getDimStint();
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
                return QString("Subject");
            case 2:
                return QString("Phrase");
            case 3:
                return QString("Status");
            case 4:
                return QString("Repeats");
            case 5:
                return QString("Hilite");
            case 6:
                return QString("Dim");
            }
        }
    }
    return QVariant();
}

bool SessionsModel::removeRows(int row, int count, const QModelIndex &parent = QModelIndex()){
    if(parent!=QModelIndex()){
        //we don't handle that
        qDebug()<<"SessionsModel: parent is something";
        return false;
    }
    bool deleted = true;
    beginRemoveRows(QModelIndex(), row, row+count-1);
    for(; count-- > 0; ++row){
        if(row > sessions.size()){
            deleted = false;
            break;
        }
        P3SessionInfo* info = this->sessions.takeAt(row);
        delete info;
    }
    endRemoveRows();
    return deleted;
}

/**
 * @brief SessionsModel::slotLoadSession
 * @param nameroots absolute file paths without the trailing _summary/_data/_meta/_targets
 */
void SessionsModel::slotLoadSession(QStringList nameroots){
    OctaveProxy* proxy = Master::getInstance()->getOctaveProxy();
    for(int i=0; i<nameroots.length(); ++i){
        beginInsertRows(QModelIndex(), this->sessions.length(), this->sessions.length());

        QString subject;
        QString phrase;
        QDateTime created;
        QString dimStint;
        QString highlightStint;
        QString repeats;
        bool clean = parseSummaryFile(nameroots.at(i)+"_summary", subject, phrase, created, dimStint, highlightStint, repeats);

        P3SessionInfo* info = proxy->loadP3Session(nameroots.at(i));
        if(info!=NULL){
            P3SessionInfo* infoFull = new P3SessionInfo(subject, phrase, clean, created, info->getSession(),
                dimStint.toUInt(), highlightStint.toUInt(), repeats.toUShort());
            this->sessions.push_back(infoFull);
        }else{
            qWarning()<<"Problems loading session "<<nameroots.at(i)<<". Ignoring and continuing.";
        }

        endInsertRows();
        qDebug()<<"Emitting dataChanged()";
        emit dataChanged(createIndex(0, 0), createIndex(rowCount()-1, columnCount()-1));
    }
}

bool SessionsModel::parseSummaryFile(QString path, QString& subject, QString& phrase, QDateTime& created, QString& dimStint,
                                     QString& highlightStint, QString& repeats){
    QFile file(path);
    bool cleanSummary=true;
    if(file.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&file);
        QString line;
        while((line = in.readLine())!=NULL){
//          qDebug()<<"I read this: "<<line<<"from summary";
            QStringList tokens = line.split(':');
            if(tokens.length()==2){
                if(tokens.at(0).trimmed().toLower()==SpellerDumper::SUMMARY_HEADER_SUBJECT){
                    subject = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()==SpellerDumper::SUMMARY_HEADER_PHRASE){
                    phrase  = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()==SpellerDumper::SUMMARY_HEADER_ERROR){
                    cleanSummary=false;
                }else if(tokens.at(0).trimmed().toLower()==SpellerDumper::SUMMARY_HEADER_DIM_STINT){
                    dimStint  = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()==SpellerDumper::SUMMARY_HEADER_HIGHLIGHT_STINT){
                    highlightStint  = tokens.at(1).trimmed();
                }else if(tokens.at(0).trimmed().toLower()==SpellerDumper::SUMMARY_HEADER_REPEATS){
                    repeats  = tokens.at(1).trimmed();
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
