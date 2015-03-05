#ifndef CLASSIFIERINFO_H
#define CLASSIFIERINFO_H

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>

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
