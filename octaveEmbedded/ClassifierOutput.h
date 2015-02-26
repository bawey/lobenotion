#ifndef CLASSIFIEROUTPUT_H
#define CLASSIFIEROUTPUT_H

class ClassifierOutput{
public:

    ClassifierOutput(unsigned short rowNumber, unsigned short columnNumber, float confidenceValue):
        confidence(confidenceValue), row(rowNumber), column(columnNumber)
    {

    }

    const float confidence;
    const unsigned short row;
    const unsigned short column;
};

#endif // CLASSIFIEROUTPUT_H
