#ifndef SPELLERCONTROLLER_H
#define SPELLERCONTROLLER_H

#include <QObject>
#include <QString>
#include <QVector>

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
    explicit SpellerController(unsigned short int matrixSize=6, QObject *parent = 0);

public:
    QVector<short> *blockRandomizeFlashes();

private:
    unsigned short int matrixSize = 0;
    QString phraseToSpell = NULL;


signals:

    /** signals the start and the end of data-taking/online mode, so that other components can adjust **/
    void dataTakingStart();
    void dataTakingEnd();

    void onlineStart();
    void onlineEnd();



public slots:
    /** external components can fireup the data taking with custom params **/
    void startDataTaking(QString phrase, int epochsPerStimuli, int interStimulusInterval, int highlightDuration, int infoDuration);
    void endDataTaking();

    void startOnline(int epochsPerStimuli, int interStimulusInterval, int highlightDuration, int infoDuration);
    void endOnline();
};

#endif // SPELLERCONTROLLER_H
