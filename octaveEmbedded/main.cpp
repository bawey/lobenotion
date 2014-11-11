#include <QCoreApplication>
#include <octaveproxy.h>
#include <QList>
#include <QVector>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    OctaveProxy proctave;
    //proctave.demo();\

    QVector<int> ints;
    ints.push_back(11);

    QList<unsigned short>* sessions = new QList<unsigned short>();
    sessions->push_back(11);
    //sessions->push_back(9);
    proctave.simpleTrainModel("/home/bawey/Desktop/eeg", "tomek", *sessions);
    sessions->removeFirst();
    sessions->push_back(12);
    proctave.simpleClassifySessions("/home/bawey/Desktop/eeg", "tomek", *sessions);
    return a.exec();
}
