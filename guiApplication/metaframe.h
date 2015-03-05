#ifndef EEGMAINTENANCEFRAME_H
#define EEGMAINTENANCEFRAME_H

class MetaFrame
{
public:
    MetaFrame(unsigned char newCounter, char newGyroX, char newGyroY, unsigned char newBattery,
              float af3, float af4, float f3, float f4, float f7, float f8, float fc5, float fc6,
              float t7, float t8, float p7, float p8, float o1, float o2);

public:

    const unsigned char counter; //loops from 0 to 128 (129 values)
    const long timestamp;
    const char gyroX, gyroY;
    const unsigned char battery; //percentage of full charge, read on counter=128

    struct ContactQuality{
        const float F3, FC6, P7, T8, F7, F8, T7, P8, AF4, F4, AF3, O2, O1, FC5;
        ContactQuality(float af3, float af4, float f3, float f4, float f7, float f8, float fc5, float fc6, float t7, float t8,
                       float p7, float p8, float o1, float o2) :
            F3(f3), FC6(fc6), P7(p7), T8(t8), F7(f7), F8(f8), T7(t7), P8(p8),
            AF4(af4), F4(f4), AF3(af3), O2(o2), O1(o1), FC5(fc5) { }
    } ;

    const ContactQuality quality;

    float getQuality(int i){
        switch (i){
        case 0:
            return this->quality.AF3;
        case 1:
            return this->quality.AF4;
        case 2:
            return this->quality.F3;
        case 3:
            return this->quality.F4;
        case 4:
            return this->quality.F7;
        case 5:
            return this->quality.F8;
        case 6:
            return this->quality.FC5;
        case 7:
            return this->quality.FC6;
        case 8:
            return this->quality.T7;
        case 9:
            return this->quality.T8;
        case 10:
            return this->quality.P7;
        case 11:
            return this->quality.P8;
        case 12:
            return this->quality.O1;
        case 13:
            return quality.O2;
        default:
            return .0;
        }
    }


};

#endif // EEGMAINTENANCEFRAME_H
