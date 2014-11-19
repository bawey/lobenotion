#ifndef CLASSIFIERINFO_H
#define CLASSIFIERINFO_H

struct ClassifierInfo{
    QString subject;
    QString parameters;
    unsigned short charsCount;
    octave_value classifier;
    octave_value tdMean;
    octave_value tdStd;
};

#endif // CLASSIFIERINFO_H
