#ifndef OCTAVEPROXY_H
#define OCTAVEPROXY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <iostream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <octaveEmbedded/p3sessioninfo.h>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <octaveEmbedded/octaveoutputreader.h>

#define DOWNSAMPLING_RATE 6

#define P3MultiSession "P3SessionLobeShorthand"
#define P3SingleSession "P3SessionLobenotion"

class OctaveProxy : public QObject
{
    Q_OBJECT
public:
    explicit OctaveProxy(QObject *parent = 0);
    ~OctaveProxy();

    octave_value pickBestModel(QList<const P3SessionInfo*>);

signals:

    void signalFetchedOutput(QString content);

public slots:
    void demo();

    void simpleTrainModel(QString dirpath, QString subject, QList<unsigned short> sessions);
    void simpleClassifySessions(QString dirpath, QString subject, QList<unsigned short> sessions);
    void interpreter();
    void pickBestModel(QString dirpath, QString subject, QList<unsigned short> sessions);


    void diag();

    octave_value loadMergeAndDownsample(QString dirpath, QString subject, QList<unsigned short> sessions);
    octave_value_list loadSessions(QString dirpath, QStringList nameRoots);
    octave_value_list loadSessions(QStringList nameRoots);


    void slotSocketConnected(){
        qDebug()<<"socket connected!";
    }

    void slotBytesWritten(qint64 bytes){
        qDebug()<<"written "<<bytes<<" bytes";
    }

    void slotSocketReadout(){
        qDebug()<<"Timer timeout fired!";
        QByteArray barray = socket.readAll();
        QString readout = QString::fromUtf8(barray.constData());
        qDebug()<<"socket readout: "<<readout;
    }

    void slotFetchedOctaveOutput(QString output){
        emit(signalFetchedOutput(output));
    }

private:

    octave_value model;
    octave_value tr_mean;
    octave_value tr_std;

    QLocalServer server;
    QLocalSocket socket;

    QThread outputThread;
    OctaveOutputReader* outputReader;
};

#endif // OCTAVEPROXY_H
