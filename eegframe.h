#ifndef EEGFRAME_H
#define EEGFRAME_H
#include <iostream>


class EegFrame
{
public:

    static const int CONTACTS_NO=14;
    static const std::string contacts[CONTACTS_NO];

    EegFrame(int af3, int af4, int f3, int f4, int f7, int f8, int fc5, int fc6, int t7, int t8,
             int p7, int p8, int o1, int o2);
    const long timestamp;
    const int F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5; //raw data values

    friend std::ostream& operator<<(std::ostream& out, const EegFrame& frame);

    int getValue(int i){
        switch (i){
        case 0:
            return AF3;
        case 1:
            return AF4;
        case 2:
            return F3;
        case 3:
            return F4;
        case 4:
            return F7;
        case 5:
            return F8;
        case 6:
            return FC5;
        case 7:
            return FC6;
        case 8:
            return T7;
        case 9:
            return T8;
        case 10:
            return P7;
        case 11:
            return P8;
        case 12:
            return O1;
        case 13:
            return O2;
        default:
            return 0;
        }
    }
};

#endif // EEGFRAME_H
