#ifndef SPELLERDUMPER_H
#define SPELLERDUMPER_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <eegframe.h>
#include <spellercontroller.h>
#include <QVector>

struct dataTakingParams;

class SpellerDumper : public QObject
{
    Q_OBJECT

public:
    static constexpr char const* SUMMARY_HEADER_ERROR="errcode";
    static constexpr char const* SUMMARY_HEADER_SUBJECT="subject";
    static constexpr char const* SUMMARY_HEADER_PHRASE="phrase";
    static constexpr char const* SUMMARY_HEADER_DIM_STINT="dim";
    static constexpr char const* SUMMARY_HEADER_HIGHLIGHT_STINT="highlight";
    static constexpr char const* SUMMARY_HEADER_REPEATS="repeats";

private:
    short waitForCompleteEpoch = 0;
    bool isOnline = false;
    unsigned int onlineDataSize = 0;
    unsigned int onlineMetaSize = 0;
    bool onlinePeriodOngoing = false;

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

    QVector<int>* onlineMeta;
    QVector<int>* onlineData;
    QVector<int>* onlineTrg;

    QSharedPointer<QVector<int>> epochMeta;
    QSharedPointer<QVector<int>> epochData;
    QSharedPointer<QVector<int>> epochTarg;


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

    void startOnlineMode(const dataTakingParams *params);
    void closeOnlineMode();

    void startOnlinePeriod();
    void closeOnlinePeriod();

    void slotOnlineEpochEnded();

signals:
   void onlinePeriodCaptured(QSharedPointer<QVector<int>> data, QSharedPointer<QVector<int>> meta, QSharedPointer<QVector<int>> targets);
   void signalOnlineEpochCaptured(QSharedPointer<QVector<int>> data, QSharedPointer<QVector<int>> meta, QSharedPointer<QVector<int>> targets);
};

#endif // SPELLERDUMPER_H
