#ifndef SPELLERDUMPER_H
#define SPELLERDUMPER_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <eegframe.h>

class SpellerDumper : public QObject
{
    Q_OBJECT

private:
    QDir* parentDir = 0;
    QFile* dataFile = 0;
    QFile* metaFile = 0;
    QFile* targetsFile = 0;
    QTextStream* dataStream = 0;
    QTextStream* metaStream = 0;
    QTextStream* targetsStream = 0;
    unsigned short targetRow = 0;
    unsigned short targetColumn = 0;

    void determineParentDirectory(QString suggestedPath);

public:
    SpellerDumper();
    ~SpellerDumper();

public slots:

    void eegFrame(QSharedPointer<EegFrame> eegFrame);

    void spellerHint(short row, short column);
    void spellerHighlight(short i);

    void closeDumpingSession();
    void startDumpingSession(QString subjectName, QString parentDirPath);
};

#endif // SPELLERDUMPER_H
