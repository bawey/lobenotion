#include "p3sessioninfo.h"

P3SessionInfo::P3SessionInfo(QString subject, QString spelledPhrase, bool isClean, QDateTime fileCreationTime, octave_value octaveSession,
                             unsigned int dimStint, unsigned int highlightStint, unsigned short repeats)
    : subjectName(subject), phrase(spelledPhrase), loadedSession(octaveSession), clean(isClean), created(fileCreationTime),
      stintDim(dimStint), stintHighlight(highlightStint), epochsPerStimulus(repeats)
{
}

P3SessionInfo::P3SessionInfo(octave_value octaveSession)
    : subjectName("N/A"), phrase(""), loadedSession(octaveSession), clean(true), created(QDateTime())
{

}
