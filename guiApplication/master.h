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
#include <sessionsmodel.h>
#include <classifiersmodel.h>
#include <octaveEmbedded/octaveproxy.h>
#include <QThread>

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

    const MetaProcessor* getMetaProcessor() const{
        return metaProcessor;
    }

    const SpellerController* getSpellerController() const{
        return spellerController;
    }

    SessionsModel* getSessionsModel() const{
        return sessionsModel;
    }

    ClassifiersModel* getClassifiersModel() const{
        return classifiersModel;
    }

    OctaveProxy* getOctaveProxy(){
        return octaveProxy;
    }

    void doGlobalInitialization(){
        qsrand((uint)QTime::currentTime().msec());
        Timer::start();
    }

private:
    explicit Master(QObject *parent = 0);
    virtual ~Master();
    static Master* instance;
    static QMutex mutex;

    EegDaq* daq = NULL;

    MetaProcessor* metaProcessor;
    SpellerController* spellerController;
    SessionsModel* sessionsModel;
    ClassifiersModel* classifiersModel;
    OctaveProxy* octaveProxy;
    bool started = false;
    QThread *workerThread;

    void connectModules();
    void instantiateSettings();
    void recreateDaq();

signals:
    void signalErrorRelay(QString errmsg);
    void signalNewDaq(const EegDaq* daq);
    void signalLaunchDaq();
public slots:
    void slotErrorRelay(QString errmsg);
    void slotConfigChanged(QString property);
    void start();
};

#endif // MASTER_H
