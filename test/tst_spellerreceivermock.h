#ifndef TST_SPELLERRECEIVERMOCK_H
#define TST_SPELLERRECEIVERMOCK_H

#include <QObject>

class SpellerReceiverMock : public QObject
{
    Q_OBJECT
public:
    explicit SpellerReceiverMock(QObject *parent = 0);

signals:

public slots:
    void highlightRowCol(short);
    void displayInformation(QString);
    void indicateTarget(short, short);
    void dim();
    void dataTakingEnded();
};

#endif // TST_SPELLERRECEIVERMOCK_H
