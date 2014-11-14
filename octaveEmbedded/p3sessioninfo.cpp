#include "p3sessioninfo.h"

P3SessionInfo::P3SessionInfo(QString subject, QString spelledPhrase, bool isClean, QDateTime fileCreationTime, octave_value octaveSession)
    : subjectName(subject), phrase(spelledPhrase), loadedSession(octaveSession), clean(isClean), created(fileCreationTime)
{
}
