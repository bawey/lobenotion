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
    void signalStartDataTaking(QString phrase, int epochsPerStimulus, int interStimulusInterval, int interPeriodInterval, int highlightDuration, int infoDuration);
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

    QThread::currentThread()->setObjectName("TestSpellerControllerThread");

    this->controller = new SpellerController(matrixSize, keyboardSymbols);

    qsrand((uint)QTime::currentTime().msec());
    mockThread = new QThread();
    mockThread->setObjectName("ReceiverMockThread");
    receiverMock = new SpellerReceiverMock();
    receiverMock->moveToThread(mockThread);
    QObject::connect(controller, SIGNAL(dataTakingEnded()), receiverMock, SLOT(dataTakingEnded()));
    bool connectStarted = QObject::connect(mockThread, SIGNAL(started()), this, SLOT(mockThreadStarted()));//, Qt::ConnectionType::BlockingQueuedConnection);
    bool connectEnded = QObject::connect(mockThread, SIGNAL(finished()), this, SLOT(mockThreadFinished()));//, Qt::ConnectionType::BlockingQueuedConnection);
    qDebug()<<"mockThread started and finished connected successfully: "<<connectStarted<<" and "<<connectEnded;
    mockThread->start();

    QObject::connect(this, SIGNAL(signalStartDataTaking(QString,int,int,int,int,int)), controller, SLOT(startDataTaking(QString,int,int,int,int,int)),
                     Qt::ConnectionType::BlockingQueuedConnection);

    QObject::connect(this, SIGNAL(signalTerminateControllerThread()), controller, SLOT(terminate()), Qt::ConnectionType::BlockingQueuedConnection);

    qDebug()<<"SpellerControllerTest constructed!";
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
    emit signalStartDataTaking("OJEJ", 3, 10, 100, 6, 30);
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
