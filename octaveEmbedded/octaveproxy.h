#ifndef OCTAVEPROXY_H
#define OCTAVEPROXY_H

#include <QObject>
#include <QString>
#include <QList>
#include <iostream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>

#define DOWNSAMPLING_RATE 6

#define P3MultiSession "P3SessionLobeShorthand"

class OctaveProxy : public QObject
{
    Q_OBJECT
public:
    explicit OctaveProxy(QObject *parent = 0);
    ~OctaveProxy();

signals:

public slots:
    void demo();

    void simpleTrainModel(QString dirpath, QString subject, QList<unsigned short> sessions);
    void simpleClassifySessions(QString dirpath, QString subject, QList<unsigned short> sessions);


    void diag();

private:
    octave_value model;
    octave_value tr_mean;
    octave_value tr_std;
};

#endif // OCTAVEPROXY_H
