#include <QString>
#include <QtTest>
#include "spellercontroller.h"
#include <QVector>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QTime>
#include <QThread>
#include <QApplication>
#include <mockapplication.h>
#include <tst_spellerreceivermock.h>
#include <QMap>
#include <QPair>
#include <eegdaq.h>
#include <fakedaq.h>

/**
 *  Some behind-the-scenes sorcery prevents slots in this object's thread from being called.
 *  Hence the mock is moved to a separate thread and will somehow be accessed to see if all the elements appeared as many times as required.
 *
 * @brief The TestSpellerController class
 */

class TestSpellerController : public QObject
{
    Q_OBJECT

public:
    TestSpellerController();
    ~TestSpellerController();

private:
    unsigned short int matrixSize=6;
    QString keyboardSymbols = QString("A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9 0");
    SpellerController* controller;

    MockApplication* app;
    QThread* mockThread;
    SpellerReceiverMock* receiverMock;

private Q_SLOTS:

    void testPhraseValidation();
    void testBlockRandomize();
    void testColRowToAndFromNumber();
    void testStartDataTaking();

    void cleanupTestCase();

/** For testing the slot-dependent features **/
signals:
    void signalStartDataTaking(QString phrase, int epochsPerStimulus, int interStimulusInterval, int interPeriodInterval,
                               int highlightDuration, int infoDuration, QString subjectName, QString parentDir);
    void signalTerminateControllerThread();

public slots:
    void mockThreadStarted(){
        qDebug()<<"MockThread is running and "<<QThread::currentThread()->objectName()<<" thread is notified about it";
    }

    void mockThreadFinished(){
        qDebug()<<"MockThread is done and "<<QThread::currentThread()->objectName()<<" thread is notified about it";
    }
};

TestSpellerController::TestSpellerController()
{
    /** some components will need an events loop **/
    int argc=0;
    char *argv[0];
    app = new MockApplication(argc, argv);

    EegDaq* daq = new FakeDaq();
    daq->start();

    QThread::currentThread()->setObjectName("TestSpellerControllerThread");

    this->controller = new SpellerController(daq, matrixSize, keyboardSymbols);

    qsrand((uint)QTime::currentTime().msec());
    mockThread = new QThread();
    mockThread->setObjectName("ReceiverMockThread");
    receiverMock = new SpellerReceiverMock();
    receiverMock->moveToThread(mockThread);


    QObject::connect(mockThread, SIGNAL(started()), receiverMock, SLOT(initializeMembers()));

    mockThread->start();

    //Connect all the signals-slots

    QObject::connect(controller, SIGNAL(dataTakingEnded()), receiverMock, SLOT(dataTakingEnded()));
    QObject::connect(this, SIGNAL(signalStartDataTaking(QString,int,int,int,int,int)), controller, SLOT(startDataTaking(QString,int,int,int,int,int, QString, QString)),
                     Qt::ConnectionType::BlockingQueuedConnection);

    QObject::connect(this, SIGNAL(signalTerminateControllerThread()), controller, SLOT(terminate()), Qt::ConnectionType::BlockingQueuedConnection);

    connect(controller, SIGNAL(commandDimKeyboard()), receiverMock, SLOT(dim()));
    connect(controller, SIGNAL(commandRowColHighlight(short)), receiverMock, SLOT(highlightRowCol(short)));
    connect(controller, SIGNAL(commandShowMessage(QString)), receiverMock, SLOT(displayInformation(QString)));
    connect(controller, SIGNAL(commandNextPeriod()), receiverMock, SLOT(nextPeriod()));
    connect(controller, SIGNAL(commandIndicateTarget(short,short)), receiverMock, SLOT(indicateTarget(short,short)));

    //qDebug()<<"TestSpellerController constructed!";
}
void TestSpellerController::testPhraseValidation()
{
    QVERIFY2(controller->validateInputString("PIES"), "PIES is spellable using the character set provided");
    QVERIFY2(controller->validateInputString("CAT0123"), "CAT0123 is spellable using the character set provided");
    QVERIFY2(!controller->validateInputString("2_21"), "2_21 is NOT spellable using the character set provided");
}

void TestSpellerController::testColRowToAndFromNumber(){

    unsigned short int columns[] =  {1, 3, 6, 5};
    unsigned short int rows[] =     {1, 2, 6, 2};
    unsigned short int numbers[] =  {0, 8, 35, 10};

    for(unsigned short int i=0; i<sizeof(numbers)/sizeof(unsigned short int); ++i){
        unsigned short int col=0;
        unsigned short int row=0;

        controller->symbolNumberToRowCol(numbers[i], row, col);

        QCOMPARE(col, columns[i]);
        QCOMPARE(row, rows[i]);
        QCOMPARE(controller->symbolRowColToNumber(row, col), numbers[i]);
    }

}

void TestSpellerController::testBlockRandomize(){
    QVector<short int>* indices = controller->blockRandomizeFlashes();

    QString stringified="";
    for(int i=0; i<indices->size(); ++i){
        stringified+=QString::number(indices->at(i))+" ";
    }

    qDebug()<<"Randomized sequence: "<<stringified;


    QCOMPARE(2*matrixSize, indices->size());

    //make sure rows and columns interchange
    for(int i=0; i<indices->size(); i+=2){
        QString message = QString("Two rows or two columns back to back: %1 and %2").arg(QString::number(indices->at(i)), QString::number(indices->at(i+1)));
        QVERIFY2(indices->at(i)*indices->at(i+1)<0, message.toStdString().c_str());
    }

    // make sure it is either always a column or always a row that comes first in a block
    for(int i=0; i<indices->size(); i+=2){
        QString message = QString("Firsts of each pair needs the same sign: %1, %2").arg(QString::number(indices->at(0)), QString::number(indices->at(i)));
        QVERIFY2(indices->at(0)*indices->at(i)>0, message.toStdString().c_str());
    }

    for(int i=1; i<indices->size(); i+=2){
        QString message = QString("Seconds of each pair needs the same sign: %1, %2").arg(QString::number(indices->at(1)), QString::number(indices->at(i)));
        QVERIFY2(indices->at(1)*indices->at(i)>0, message.toStdString().c_str());
    }


    //make sure there is some randomization among both the rows and the columns
    float monotonicityCoeff=0;

    for(int i=2; i<indices->size(); i+=1){
        monotonicityCoeff+=qAbs(indices->at(i)-indices->at(i-2));
    }
    monotonicityCoeff/=(indices->length()-2);
    qDebug()<<"m. coeff: "<<monotonicityCoeff;
    QString message = QString("This order seems too much in order: ")+stringified+QString(" [monotonicity coeff: %1]").arg(monotonicityCoeff);
    QVERIFY2(monotonicityCoeff>1.5, message.toStdString().c_str());

    //make sure the result varies between invocations
    QVector<short int>* nextIndices = controller->blockRandomizeFlashes();
    bool thereWasADifference=false;
    for(int i=0; i<indices->size(); ++i){
        thereWasADifference=thereWasADifference || indices->at(i)!=nextIndices->at(i);
    }
    message=QString("Got the same vector twice in a row: ")+stringified;
    QVERIFY2(thereWasADifference, message.toStdString().c_str());
}

void TestSpellerController::testStartDataTaking(){
    qDebug("Trying to emit a signal to Controller");
    //blocking connection ensures we can "examine" the mock right after handling the signal

    QString phrase = "OJEJ";
    unsigned short flashesPerStimulus = 3;
    unsigned short interStimulusInterval = 10;
    unsigned short interPeriodInterval = 100;
    unsigned short highlightDuration = 6;
    unsigned short informationDuration = 30;

    emit signalStartDataTaking(phrase, flashesPerStimulus, interStimulusInterval, interPeriodInterval,
                               highlightDuration, informationDuration, "MockSubject", QDir::tempPath()+"/mockEeg");

    QCOMPARE((int) receiverMock->periodsCount, phrase.length());

    unsigned short expectedDims = matrixSize*2*flashesPerStimulus+2;
    QString targetPhrase="";
    for(int i=0; i<receiverMock->periodsCount; ++i){
        QCOMPARE(receiverMock->dimsInPeriods->at(i), expectedDims);
        QMap<short, unsigned short>* stimuliHistogram = receiverMock->stimuliInPeriods->at(i);
        for(short key = -matrixSize; key<=matrixSize; key==-1 ? key+=2 : ++key){
            //qDebug()<<"In period "<<i<<" stimuli code "<<key<<" appeared "<<stimuliHistogram->value(key)<<" times";
            QVERIFY2(stimuliHistogram->contains(key), QString("Stimuli code %1 not present!").arg(QString::number(key)).toStdString().c_str());
            QVERIFY2(stimuliHistogram->value(key)==flashesPerStimulus, "Strange number of flashes per stimulus");
        }
        QPair<unsigned short, unsigned short>* rowCol = receiverMock->targets->at(i);
        unsigned short symbolNumber = controller->symbolRowColToNumber(rowCol->first, rowCol->second);
        targetPhrase.append(keyboardSymbols.at(symbolNumber*2));
    }
    QCOMPARE(phrase, targetPhrase);
    qDebug()<<"Fine, each of "<<receiverMock->periodsCount<<" periods contained "<<expectedDims<<
              " requests to dim the keyboard, each stimulus was highlighted "<<flashesPerStimulus<<
              "times and target phrase was: "<<targetPhrase;

    /** Check if Mock received the 'session complete' signal and if in a correct thread
     */
    QVERIFY2(receiverMock->notifiedAboutSessionEnd, "Mock object has not been notified about the session end");
    QVERIFY(receiverMock->sessionEndReceivedByThreadName!=QThread::currentThread()->objectName());
    QVERIFY(receiverMock->sessionEndReceivedByThreadName!=QString(""));

}

void TestSpellerController::cleanupTestCase(){
    emit signalTerminateControllerThread();
}

TestSpellerController::~TestSpellerController(){
   controller->deleteLater();

   mockThread->quit();
   mockThread->wait();
   delete receiverMock;
   delete mockThread;
}


QTEST_APPLESS_MAIN(TestSpellerController)

#include "tst_testspellercontroller.moc"
