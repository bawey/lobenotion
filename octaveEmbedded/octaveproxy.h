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
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <octaveEmbedded/octaveoutputreader.h>
#include <QSharedPointer>
#include <QVector>
#include <QPair>
#include <classifierinfo.h>
#include <octaveEmbedded/ClassifierOutput.h>
#include <QMutex>

#define P3MultiSession "P3SessionLobeShorthand"
#define P3SingleSession "P3SessionLobenotion"

class OctaveProxy : public QObject
{
    Q_OBJECT
public:
    explicit OctaveProxy(bool redirectOutput=true, QObject *parent = 0);
    ~OctaveProxy();

    ClassifierInfo *pickBestModel(QList<const P3SessionInfo*>);

    QSharedPointer<QVector<ClassifierOutput*>> askClassifier(const ClassifierInfo* modelDesc, QList<const P3SessionInfo *>* );
    QSharedPointer<QVector<ClassifierOutput*>> askClassifier(const ClassifierInfo* modelDesc, const P3SessionInfo* sessionDesc);

signals:

    void signalFetchedOutput(QString content);
    void signalOctaveError(QString errmsg);
    void signalOctaveBusy(bool busy);

public slots:
    void interpreter();

    P3SessionInfo* loadP3Session(QString absNameroot);

    P3SessionInfo* toP3Session(
            QSharedPointer<QVector<int>> signal, QSharedPointer<QVector<int>> meta, QSharedPointer<QVector<int>> targets,
            int channelsCount=14, int samplingRate=128,
            QString channelNames="{'F3', 'FC6', 'P7', 'T8', 'F7', 'F8', 'T7', 'P8', 'AF4', 'F4', 'AF3', 'O2', 'O1', 'FC5'}");

    void slotFetchedOctaveOutput(QString output){
        emit(signalFetchedOutput(output));
    }

    void slotReloadScripts();

    void analyzeConfidence(const ClassifierInfo* model, QSharedPointer<QList<const P3SessionInfo *>> data);

private:

    QMutex mutex;
    QThread outputThread;
    OctaveOutputReader* outputReader;

    bool errorCheckEpilogue();
    /**
     * @brief mergedSession used to merge several sessions into one when starting a training
     * @return
     */
    octave_value mergedSession(QList<const P3SessionInfo *>*);

    QSharedPointer<QVector<ClassifierOutput*>> askClassifier(const octave_value *classifier, const octave_value* session);
};

#endif // OCTAVEPROXY_H
