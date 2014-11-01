#include <QString>
#include <QtTest>
#include "spellercontroller.h"
#include <QVector>
#include <QString>
#include <iostream>

class TestSpellerController : public QObject
{
    Q_OBJECT

public:
    TestSpellerController();
    ~TestSpellerController();

private:
    unsigned short int matrixSize=6;
    SpellerController* controller;

    QTextStream* output;
    QFile* file;

private Q_SLOTS:
    void testConnection();
    void testBlockRandomize();
    void cleanupTestCase();
};

TestSpellerController::TestSpellerController()
{
    QString filename="/tmp/TestSpellerController_extra.output";
    file = new QFile( filename );
    if ( file->open(QIODevice::Append) )
    {
        this->output = new QTextStream( file );
        *output << "Stream created now and now" << endl;
    }
    this->controller = new SpellerController(matrixSize);
}
void TestSpellerController::testConnection()
{
    *output<<"dummy test and dummy output"<<endl;
    QVERIFY2(true, "Failure");
}

void TestSpellerController::testBlockRandomize(){
    *output<<"attempting another test"<<endl;

    QVector<short int>* indices = controller->blockRandomizeFlashes();

    QString stringified="";
    for(int i=0; i<indices->size(); ++i){
        stringified+=QString::number(indices->at(i))+" ";
    }

    *output<<stringified<<endl;
    output->flush();

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
    int unorderedCount=0;

    for(int i=2; i<indices->size(); i+=1){
        if(indices->at(i)<indices->at(i-2)){
            ++unorderedCount;
        }
    }
    QString message = QString("This order seems too much in order: ")+stringified;
    QVERIFY2(unorderedCount>indices->length()/3, message.toStdString().c_str());

    //make sure the result varies between invocations
    QVector<short int>* nextIndices = controller->blockRandomizeFlashes();
    bool thereWasADifference=false;
    for(int i=0; i<indices->size(); ++i){
        thereWasADifference=thereWasADifference || indices->at(i)!=nextIndices->at(i);
    }
    message=QString("Got the same vector twice in a row: ")+stringified;
    *output<<"the next time we got: "<<nextIndices->at(0)<<" "<<nextIndices->at(1)<<"..."<<endl;
    QVERIFY2(thereWasADifference, message.toStdString().c_str());
}

void TestSpellerController::cleanupTestCase(){
    output->flush();
    file->close();
    delete output;
    delete file;
    delete controller;
}

TestSpellerController::~TestSpellerController(){

}

QTEST_APPLESS_MAIN(TestSpellerController)

#include "tst_testspellercontroller.moc"
