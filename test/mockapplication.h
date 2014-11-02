#ifndef MOCKAPPLICATION_H
#define MOCKAPPLICATION_H

#include <QApplication>

class MockApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MockApplication(int& argc, char** argv);

private:
    bool notify(QObject *receiver_, QEvent *event_);

signals:

public slots:

};

#endif // MOCKAPPLICATION_H
