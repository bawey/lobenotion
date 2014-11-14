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

#define DOWNSAMPLING_RATE 6

#define P3MultiSession "P3SessionLobeShorthand"
#define P3SingleSession "P3SessionLobenotion"

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
    void interpreter();
    void pickBestModel(QString dirpath, QString subject, QList<unsigned short> sessions);


    void diag();

    octave_value loadMergeAndDownsample(QString dirpath, QString subject, QList<unsigned short> sessions);
    octave_value_list loadSessions(QString dirpath, QStringList nameRoots);
    octave_value_list loadSessions(QStringList nameRoots);
private:

    octave_value model;
    octave_value tr_mean;
    octave_value tr_std;
};

#endif // OCTAVEPROXY_H
