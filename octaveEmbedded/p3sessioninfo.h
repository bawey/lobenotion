#ifndef P3SESSIONINFO_H
#define P3SESSIONINFO_H

#include <QString>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <QDateTime>

/**
 * This class stores basic information regarding P3 session: data taking parameters, location
 * and an octave_value should the session be already loaded into teh memory
 */
class P3SessionInfo
{
public:
    P3SessionInfo(QString subject, QString spelledPhrase, bool clean, QDateTime fileCreationTime, octave_value octaveSession);

    const QString& getSubjectName(){
        return subjectName;
    }

    const QString& getPhrase(){
        return phrase;
    }

    octave_value getSession(){
        return loadedSession;
    }

    bool isClean(){
        return clean;
    }

    QDateTime getCreated(){
        return created;
    }

private:
    octave_value loadedSession;

    //these are copied from dataTakingParams and it should be somehow factored out into class hierarchy
    unsigned int stintHighlight;
    unsigned int stintDim;
    unsigned int stintInfo;
    unsigned int stintInterPeriod;
    unsigned short epochsPerStimulus;
    QString phrase;
    QString subjectName;
    QString parentDir;
    QDateTime created;

    bool clean;

};

#endif // P3SESSIONINFO_H
