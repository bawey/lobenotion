#include <QString>
#include <QVector>
#include <QtTest>
#include <QCoreApplication>
#include <octaveEmbedded/octaveproxy.h>
#include <QSharedPointer>
#include <QTime>

class TestOctaveEmbeddedTest : public QObject
{
    Q_OBJECT

public:
    TestOctaveEmbeddedTest();
    ~TestOctaveEmbeddedTest(){
        delete proctave;
    }

private Q_SLOTS:
    void p3SessionFromMemory();

private:
    OctaveProxy* proctave;
};

TestOctaveEmbeddedTest::TestOctaveEmbeddedTest()
{
    qsrand(QTime::currentTime().msec());
    proctave = new OctaveProxy(false);
}

void TestOctaveEmbeddedTest::p3SessionFromMemory()
{
    int stimuli[]={-6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6};

    QVector<int>* data = new QVector<int>();
    QVector<int>* meta = new QVector<int>();
    QVector<int>* targ = new QVector<int>();

    QSharedPointer<QVector<int>> dataPtr(data);
    QSharedPointer<QVector<int>> metaPtr(meta);
    QSharedPointer<QVector<int>> targPtr(targ);

    targ->append(0);targ->append(0);targ->append(0);

    int edur = 400;

    for(int epoch=0;epoch<150; ++epoch){
        //highlights are emitted for a while, then comes a break. during the break a few frames need to be captured as well!
        if(epoch<144){
            meta->append(epoch*edur);

            int stimulus=stimuli[qrand()%12];

            meta->append(qAbs(stimulus==3)?1:0);
            meta->append(stimulus);
        }
        for(int time=0; time<edur; time+=7){
            data->append(epoch*edur+time);
            data->append(qrand());
            data->append(qrand());
            data->append(qrand());
        }
    }


    proctave->p3Session(dataPtr, metaPtr, targPtr, 3, 128, "{'channelName1', 'channelName2', 'lastChannelName'}");
}

QTEST_MAIN(TestOctaveEmbeddedTest)

#include "tst_testoctaveembeddedtest.moc"
