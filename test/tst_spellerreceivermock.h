#ifndef TST_SPELLERRECEIVERMOCK_H
#define TST_SPELLERRECEIVERMOCK_H
#include <QMap>
#include <QVector>
#include <QPair>
#include <QObject>

class SpellerReceiverMock : public QObject
{
    Q_OBJECT
public:
    explicit SpellerReceiverMock(QObject *parent = 0);
    ~SpellerReceiverMock();
    bool notifiedAboutSessionEnd=false;
    QString sessionEndReceivedByThreadName="";
    unsigned short periodsCount=0;
    QVector<QMap<short, unsigned short>*>* stimuliInPeriods;
    QVector<unsigned short>* dimsInPeriods;
    QVector<QPair<unsigned short, unsigned short>*>* targets;

signals:

public slots:
    void initializeMembers(){
        stimuliInPeriods = new QVector<QMap<short, unsigned short>*>();
        dimsInPeriods = new QVector<unsigned short>();
        targets = new QVector<QPair<unsigned short, unsigned short>*>();
    }
    void highlightRowCol(short);
    void displayInformation(QString);
    void indicateTarget(short, short);
    void dim();
    void dataTakingEnded();
    void nextPeriod();
};

#endif // TST_SPELLERRECEIVERMOCK_H
