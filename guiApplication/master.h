#ifndef MASTER_H
#define MASTER_H

#include <QObject>

#include <metaprocessor.h>
#include <spellercontroller.h>
#include <eegdaq.h>
#include <fakedaq.h>
#include <epocdaq.h>
#include <QMutex>
#include <QThread>
#include <timer.h>
#include <settings.h>

/**
 * @brief The Master class
 *
 * An object of this class will hold together all the components of the program and handle demands for their reconfiguration/recreation.
 *
 */

class Master : public QObject
{
    Q_OBJECT
public:
    static Master* getInstance();

    const EegDaq* const getDaq() const {
        return daq;
    }

    const MetaProcessor* const getMetaProcessor() const{
        return metaProcessor;
    }

    const SpellerController* const getSpellerController() const{
        return spellerController;
    }

    void doGlobalInitialization(){
        qsrand((uint)QTime::currentTime().msec());
        Timer::start();
    }

private:
    explicit Master(QObject *parent = 0);
    static Master* instance;
    static QMutex mutex;

    EegDaq* daq;

    MetaProcessor* metaProcessor;
    SpellerController* spellerController;


signals:

public slots:

};

#endif // MASTER_H
