#ifndef SPELLERCONTROLLER_H
#define SPELLERCONTROLLER_H

#define TEST_VISIBILITY public

#include <QObject>
#include <QString>
#include <QVector>
#include <QChar>
#include <QStringList>
#include <QThread>
#include <QPair>
#include <QDebug>
#include <QDir>
#include <QSharedPointer>
#include <eegdaq.h>
#include <spellerdumper.h>
#include <metaframe.h>

/**
 *  This class handles the data-taking and online-use scenarios of the Speller
 *  It:
 *      -   commands the widget to display stimuli
 *      -   forwards EEG frames to the dumper and/or analyzer
 *      -   handles the inilialization/cleanup of any of the scenarios
 */

class SpellerController : public QObject
{
    Q_OBJECT

public:

    constexpr static unsigned char ERRCODE_PARAMETERS = 1;
    constexpr static unsigned char ERRCODE_SIGNAL = 2;

    explicit SpellerController(EegDaq* daq, unsigned short int matrixSize, QString characters, QObject *parent = 0);
    virtual ~SpellerController();

    /**
     * @brief symbolNumerToRowCol returns the row and column numbers corresponding to 0..nSymbols-1 indexed number of symbol.
     * Columns and rows are numbered from 1 upwards
     * @param number
     * @param refRow reference to hold the row number under
     * @param refCol reference to hold the column number under
     * @return
     */
    void symbolNumberToRowCol(unsigned int number, unsigned short int &refRow, unsigned short int &refCol){
        refRow = (short unsigned int) number/matrixSize + 1;
        refCol = number%matrixSize + 1;
    }

    /**
     * @brief symbolRowColToNumber returns the 0..nSymbols-1 number of symbol (tile) at given row/col intersection
     * @param row
     * @param col
     * @return
     */
    unsigned short int symbolRowColToNumber(short int row, short int col){
        return (qAbs(row)-1)*matrixSize + qAbs(col)-1;
    }

private:
    bool signalFine=false;

    EegDaq* daq;

    QThread* thread;
    unsigned short int matrixSize = 0;
    QVector<QString> keyboardSymbols;
    QString phraseToSpell = NULL;

    SpellerDumper* dumper;
    QThread* dumperThread;

    void sleepFrameAligned(unsigned long ms);
    void connectSignalsToSlots();
    void disconnectSignalsFromSlots();

TEST_VISIBILITY:
    QVector<short> *blockRandomizeFlashes();

    bool validateInputString(QString phrase){
        if(phrase.length()>0){
            for(int i=0; i<phrase.length(); ++i){
                if(!this->keyboardSymbols.contains(phrase.mid(i,1))){
                    return false;
                }
            }
        }else{
            return false;
        }
        return true;
    }

    /**
     * A very rudimentary check to see if the values provided make sense
     * @brief validateTimings
     * @param interStimulusInterval
     * @param interPeriodInterval
     * @param highlightDuration
     * @param infoDuration
     * @return
     */
    bool validateTimings(unsigned int interStimulusInterval, unsigned int interPeriodInterval,
                         unsigned int highlightDuration, unsigned int infoDuration){
        return highlightDuration < interStimulusInterval && interPeriodInterval > interStimulusInterval &&
                infoDuration > interStimulusInterval;
    }



signals:

    /** signals the start and the end of data-taking/online mode, so that other components can adjust **/
    void dataTakingStarted(QString subjectName, QString parentDirPath);
    void dataTakingEnded();

    void onlineStarted();
    void onlineEnded();

    /** scenario-related signals **/
    void commandRowColHighlight(short rowOrCol);
    void commandShowMessage(QString msg);
    void commandDimKeyboard();

    void commandNextPeriod();
    void commandIndicateTarget(short row, short column);

    void error(unsigned char code);

public slots:
    /** external components can fireup the data taking with custom params **/
    void startDataTaking(QString phrase, int epochsPerStimuli, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration,
                         QString subjectName="DefaultSubject", QString parentDirectory=QDir::tempPath()+"/eeg.dumps/");
    void endDataTaking();

    void startOnline(int epochsPerStimuli, int interStimulusInterval, int highlightDuration, int infoDuration);
    void endOnline();

    void terminate(){
        qDebug()<<"terminating SpellerController thread from "<<QThread::currentThread()->objectName();
        thread->quit();
    }

    void eegMetaFrame(QSharedPointer<MetaFrame> metaFrame);
};

#endif // SPELLERCONTROLLER_H
