#include "settings.h"
#include <QMutex>
#include <QApplication>
#include <QDir>


QString Settings::DEF_SPELLER_CHARSET="A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9 0";

QString Settings::OPT_DUMMY_DAQ="dummyDaq";
QString Settings::OPT_DUMMY_DAQ_NOISY="dummyDaqNoisy";
QString Settings::OPT_EEG_VISUALIZER="eegVisualizer";
QString Settings::OPT_EEG_DUMP_PATH="eegDumpPath";
QString Settings::OPT_META_DUMP_PATH="metaDumpPath";
QString Settings::OPT_SPELLER_MATRIX_SIZE="spellerMatrixSize";
QString Settings::OPT_SPELLER_CHARSET="spellerCharset";
QString Settings::OPT_SPELLER_HIGHLIGHT_STINT="spellerHighlightStint";
QString Settings::OPT_SPELLER_DIM_STINT="spellerDimStint";
QString Settings::OPT_SPELLER_INTER_PERIOD_STINT="spellerInterPeriodStint";
QString Settings::OPT_SPELLER_INFO_STINT="spellerInfoStint";
QString Settings::OPT_SPELLER_EPOCHS_PER_STIMULUS="spellerEpochsPerStimulus";
QString Settings::OPT_SPELLER_PHRASE="spellerPhrase";
QString Settings::OPT_SUBJECT_NAME="subjectName";



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

// EEG VISUALIZER aka PLOT WIDGET

bool Settings::isEegVisualizerEnabled(){
    QString reply = getInstance()->value(Settings::OPT_EEG_VISUALIZER, QString("true")).toString();
    return reply.compare("true", Qt::CaseInsensitive)==0;
}

void Settings::setEegVisualizerEnabled(bool isIt){
    getInstance()->setValue(Settings::OPT_EEG_VISUALIZER, isIt);
}

// EEG DUMP PATH

QString Settings::getEegDumpPath(){
    return getInstance()->value(Settings::OPT_EEG_DUMP_PATH, QDir::tempPath()+"/eeg/").toString();
}

void Settings::setEegDumpPath(QString newPath){
    return getInstance()->setValue(Settings::OPT_EEG_DUMP_PATH, newPath);
}

// Deprecated, I think
QString Settings::getMetaDumpPath(){
    return getInstance()->value(Settings::OPT_META_DUMP_PATH, QDir::tempPath()+"/eeg").toString();
}

// SPELLER CHARSET

QString Settings::getSpellerCharset(){
    return getInstance()->value(Settings::OPT_SPELLER_CHARSET, Settings::DEF_SPELLER_CHARSET).toString();
}

//SPELLER MATRIX SIZE

unsigned short Settings::getSpellerMatrixSize(){
    return (unsigned short) getInstance()->value(Settings::OPT_SPELLER_MATRIX_SIZE, "6").toInt();
}

// SPELLER SYMBOL HIGHLIGHT DURATION

int Settings::getSpellerHighlightStint(){
    return (unsigned short) getInstance()->value(Settings::OPT_SPELLER_HIGHLIGHT_STINT, "125").toInt();
}

void Settings::setSpellerHighlightStint(int thestint){
    getInstance()->setValue(Settings::OPT_SPELLER_HIGHLIGHT_STINT, QString::number(thestint));
}

int Settings::getSpellerDimStint(){
    return (unsigned short) getInstance()->value(Settings::OPT_SPELLER_DIM_STINT, "75").toInt();
}

void Settings::setSpellerDimStint(int thestint){
    getInstance()->setValue(Settings::OPT_SPELLER_DIM_STINT, QString::number(thestint));
}

int Settings::getSpellerInfoStint(){
    return (unsigned short) getInstance()->value(Settings::OPT_SPELLER_INFO_STINT, "300").toInt();
}

void Settings::setSpellerInfoStint(int thestint){
    getInstance()->setValue(Settings::OPT_SPELLER_INFO_STINT, QString::number(thestint));
}

int Settings::getSpellerInterPeriodStint(){
    return (unsigned short) getInstance()->value(Settings::OPT_SPELLER_INTER_PERIOD_STINT, "1000").toInt();
}

void Settings::setSpellerInterPeriodStint(int thestint){
    getInstance()->setValue(Settings::OPT_SPELLER_INTER_PERIOD_STINT, QString::number(thestint));
}

int Settings::getSpellerEpochsPerStimulus(){
    return (unsigned short) getInstance()->value(Settings::OPT_SPELLER_EPOCHS_PER_STIMULUS, "15").toInt();
}

void Settings::setSpellerEpochsPerStimulus(int epochs){
    getInstance()->setValue(Settings::OPT_SPELLER_EPOCHS_PER_STIMULUS, QString::number(epochs));
}

QString Settings::getSubjectName(){
    return getInstance()->value(Settings::OPT_SUBJECT_NAME, "anonymous").toString();
}

void Settings::setSubjectName(QString name){
    getInstance()->setValue(Settings::OPT_SUBJECT_NAME, name);
}

QString Settings::getSpellerPhrase(){
    return getInstance()->value(Settings::OPT_SPELLER_PHRASE, "PAINT").toString().toUpper();
}

void Settings::setSpellerPhrase(QString phrase){
    getInstance()->setValue(Settings::OPT_SPELLER_PHRASE, phrase);
}
