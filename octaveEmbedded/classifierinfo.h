#ifndef CLASSIFIERINFO_H
#define CLASSIFIERINFO_H

#include <oct.h>
#include <octave.h>
#include <parse.h>
#include <toplev.h>

struct ClassifierInfo{
    QString subject;
    QString parameters;
    unsigned short charsCount;
    unsigned short dataRepeats;
    unsigned int dataDimStint;
    unsigned int dataHighStint;
    octave_value classifier;
    octave_value classifierCell;
};

#endif // CLASSIFIERINFO_H
