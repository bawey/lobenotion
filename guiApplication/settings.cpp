#include "settings.h"
#include <QMutex>
#include <QApplication>


QString Settings::OPT_DUMMY_DAQ="dummyDaq";
QString Settings::OPT_DUMMY_DAQ_NOISY="dummyDaqNoisy";
QString Settings::OPT_EEG_VISUALIZER="eegVisualizer";
QString Settings::OPT_EEG_DUMP_PATH="eegDumpPath";
QString Settings::OPT_META_DUMP_PATH="metaDumpPath";
QString Settings::OPT_SPELLER_MATRIX_SIZE="spellerMatrixSize";
QString Settings::OPT_SPELLER_CHARSET="spellerCharset";


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

bool Settings::isDummyDaqNoisy(){
    QString reply = getInstance()->value(Settings::OPT_DUMMY_DAQ_NOISY, QString("true")).toString();
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

QString Settings::getEegDumpPath(){
    return getInstance()->value(Settings::OPT_EEG_DUMP_PATH).toString();
}

QString Settings::getMetaDumpPath(){
    return getInstance()->value(Settings::OPT_META_DUMP_PATH).toString();
}

QString Settings::getSpellerCharset(){
    return getInstance()->value(Settings::OPT_SPELLER_CHARSET).toString();
}

unsigned short Settings::getSpellerMatrixSize(){
    return (unsigned short) getInstance()->value(Settings::OPT_SPELLER_MATRIX_SIZE).toInt();
}
