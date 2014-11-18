#ifndef SPELLERDUMPER_H
#define SPELLERDUMPER_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <eegframe.h>
#include <spellercontroller.h>

struct dataTakingParams;

class SpellerDumper : public QObject
{
    Q_OBJECT

public:
    static constexpr char const* SUMMARY_HEADER_ERROR="errcode";
    static constexpr char const* SUMMARY_HEADER_SUBJECT="subject";
    static constexpr char const* SUMMARY_HEADER_PHRASE="phrase";

private:
    QDir* parentDir = 0;
    QFile* dataFile = 0;
    QFile* metaFile = 0;
    QFile* targetsFile = 0;
    QFile* summaryFile = 0;
    QTextStream* dataStream = 0;
    QTextStream* metaStream = 0;
    QTextStream* targetsStream = 0;
    QTextStream* summaryStream = 0;
    unsigned short targetRow = 0;
    unsigned short targetColumn = 0;

    void determineParentDirectory(QString suggestedPath);

public:
    SpellerDumper();
    ~SpellerDumper();

public slots:

    void eegFrame(QSharedPointer<EegFrame> eegFrame);
    void spellerError(unsigned char errcode);

    void spellerHint(short row, short column);
    void spellerHighlight(short i);

    void closeDumpingSession();

    void startDumpingSession(dataTakingParams *params);
};

#endif // SPELLERDUMPER_H
