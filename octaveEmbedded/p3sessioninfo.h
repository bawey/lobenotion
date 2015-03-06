#ifndef P3SESSIONINFO_H
#define P3SESSIONINFO_H

#include <QString>
#include <oct.h>
#include <octave.h>
#include <parse.h>
#include <toplev.h>
#include <QDateTime>

/**
 * This class stores basic information regarding P3 session: data taking parameters, location
 * and an octave_value should the session be already loaded into teh memory
 */
class P3SessionInfo
{
public:
    P3SessionInfo(QString subject, QString spelledPhrase, bool clean, QDateTime fileCreationTime, octave_value octaveSession,
                  unsigned int dimStint, unsigned int highlightStint, unsigned short repeats);
    P3SessionInfo(octave_value octaveSession);

    const QString& getSubjectName() const{
        return subjectName;
    }

    const QString& getPhrase() const{
        return phrase;
    }

    octave_value getSession() const{
        return loadedSession;
    }

    const octave_value* getSessionPtr() const{
        return &loadedSession;
    }

    bool isClean() const{
        return clean;
    }

    QDateTime getCreated() const {
        return created;
    }

    unsigned int getDimStint() const {
        return stintDim;
    }

    unsigned int getHighlightStint() const {
        return stintHighlight;
    }

    unsigned int getRepeats() const {
        return epochsPerStimulus;
    }

private:

    //these are copied from dataTakingParams and it should be somehow factored out into class hierarchy
    QString subjectName;
    QString phrase;
    octave_value loadedSession;
    bool clean;
    QDateTime created;
    unsigned int stintDim;
    unsigned int stintHighlight;
    unsigned short epochsPerStimulus;
//    unsigned int stintInfo;
//    unsigned int stintInterPeriod;
//    QString parentDir;

};

#endif // P3SESSIONINFO_H
