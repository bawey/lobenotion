#include "settings.h"
#include <QMutex>
#include <QApplication>


QString Settings::OPT_DUMMY_DAQ="dummyDaq";
QString Settings::OPT_EEG_VISUALIZER="eegVisualizer";


Settings* Settings::instance;

Settings::Settings() :
    QSettings("/home/bawey/Forge/lobenotion/lobenotion.ini", QSettings::NativeFormat)
{
}

Settings* Settings::getInstance(){
    static QMutex mutex;
    if(Settings::instance == NULL){
        mutex.lock();
        if(Settings::instance==NULL){
            Settings::instance=new Settings();
        }
        mutex.unlock();
    }
    return Settings::instance;
}

bool Settings::isDummyModeEnabled(){
    QString reply = getInstance()->value(Settings::OPT_DUMMY_DAQ, QString("true")).toString();
    return reply.compare("true", Qt::CaseInsensitive)==0;
}

void Settings::setDummyModeEnabled(bool isIt){
    getInstance()->setValue(Settings::OPT_DUMMY_DAQ, isIt);
}

bool Settings::isEegVisualizerEnabled(){
    QString reply = getInstance()->value(Settings::OPT_EEG_VISUALIZER, QString("true")).toString();
    return reply.compare("true", Qt::CaseInsensitive)==0;
}

void Settings::setEegVisualizerEnabled(bool isIt){
    getInstance()->setValue(Settings::OPT_EEG_VISUALIZER, isIt);
}
