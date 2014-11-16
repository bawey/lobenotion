#ifndef OCTAVEOUTPUTREADER_H
#define OCTAVEOUTPUTREADER_H

#include <QObject>
#include <QString>
#include <QDebug>

#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

class OctaveOutputReader : public QObject
{
    Q_OBJECT
public:
    explicit OctaveOutputReader(QString readPath, QObject *parent = 0);

signals:
    void signalFetchedOutput(QString output);
public slots:
    void startReadingFifo();

private:
    QString inputPath;
};

#endif // OCTAVEOUTPUTREADER_H
