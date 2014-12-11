#ifndef CLASSIFIERINFO_H
#define CLASSIFIERINFO_H

struct ClassifierInfo{
    QString subject;
    QString parameters;
    unsigned short charsCount;
    unsigned short dataRepeats;
    unsigned int dataDimStint;
    unsigned int dataHighStint;
    octave_value classifier;
};

#endif // CLASSIFIERINFO_H
