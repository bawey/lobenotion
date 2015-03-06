#include "eegframe.h"
#include "timer.h"

EegFrame::EegFrame(int af3, int af4, int f3, int f4, int f7, int f8, int fc5, int fc6, int t7, int t8,
                   int p7, int p8, int o1, int o2) :
    timestamp(Timer::getTime()), F3(f3), FC6(fc6), P7(p7), T8(t8), F7(f7), F8(f8), T7(t7), P8(p8),
    AF4(af4), F4(f4), AF3(af3), O2(o2), O1(o1), FC5(fc5)
{
}

std::ostream& operator<<(std::ostream& out, const EegFrame& frame){
    return out<<"EEG frame from ["<<frame.timestamp<<"] AF3:"<<frame.AF3<<", AF4:"<<frame.AF4
        <<", F3:"   <<frame.F3<<", F4"      <<frame.F4<<", F7:" <<frame.F7<<", F8:" <<frame.F8
        <<", FC5:"  <<frame.FC5<<", FC6:"   <<frame.FC6<<", O1:"<<frame.O1<<", 02:" <<frame.O2
        <<", P7:"   <<frame.P7<<", P8:"     <<frame.P8<<", T7:" <<frame.T7<<", T8:" <<frame.T8;
}


const std::string EegFrame::contacts[14] = {"AF3", "AF4", "F3", "F4", "F7", "F8", "FC5", "FC6", "T7", "T8", "P7", "P8", "O1", "O2"};

QTextStream& operator<<(QTextStream& out, const EegFrame& frame){
    return out<<QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15\n").arg(frame.timestamp)
         .arg(frame.AF3).arg(frame.AF4).arg(frame.F3)
         .arg(frame.F4).arg(frame.F7).arg(frame.F8).arg(frame.FC5).arg(frame.FC6).arg(frame.T7).arg(frame.T8)
         .arg(frame.P7).arg(frame.P8).arg(frame.O1).arg(frame.O2);
}
