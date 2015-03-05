#include "settings.h"
#include <QMutex>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include "master.h"

const QString Settings::DEF_SPELLER_CHARSET="A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 1 2 3 4 5 6 7 8 9 0";

const QString Settings::OPT_DUMMY_DAQ="dummyDaq";
const QString Settings::OPT_DUMMY_DAQ_NOISY="dummyDaqNoisy";
const QString Settings::OPT_EEG_VISUALIZER="eegVisualizer";
const QString Settings::OPT_EEG_DUMP_PATH="eegDumpPath";
const QString Settings::OPT_META_DUMP_PATH="metaDumpPath";
const QString Settings::OPT_SPELLER_MATRIX_SIZE="spellerMatrixSize";
const QString Settings::OPT_SPELLER_CHARSET="spellerCharset";
const QString Settings::OPT_SPELLER_HIGHLIGHT_STINT="spellerHighlightStint";
const QString Settings::OPT_SPELLER_DIM_STINT="spellerDimStint";
const QString Settings::OPT_SPELLER_INTER_PERIOD_STINT="spellerInterPeriodStint";
const QString Settings::OPT_SPELLER_INFO_STINT="spellerInfoStint";
const QString Settings::OPT_SPELLER_EPOCHS_PER_STIMULUS="spellerEpochsPerStimulus";
const QString Settings::OPT_SPELLER_PHRASE="spellerPhrase";
const QString Settings::OPT_SUBJECT_NAME="subjectName";
const QString Settings::OPT_OCTAVE_SCRIPTS_ROOT="octaveScriptsRoot";
const QString Settings::OPT_TRAIN_DECIMATION_FACTOR="trainDecimationFactor";
const QString Settings::OPT_TRAIN_XV_RATE="trainXvRate";
const QString Settings::OPT_TRAIN_PERIOD_OVERSAMPLING="trainPeriodOversampling";
const QString Settings::OPT_TRAIN_BALANCING="trainBalancing";
const QString Settings::OPT_ONLINE_CONFIDENCE="onlineConfidenceThreshold";
const QString Settings::OPT_ONLINE_MIN_EPOCHS="onlineMinEpochs";

const QString Settings::OPT_QC_INTERRUPT = "qcInterrupt";
const QString Settings::OPT_QC_GOODNESS = "qcGoodness";
const QString Settings::OPT_QC_STRICTNESS = "qcStrictness";
const QString Settings::OPT_QC_CHANNELS_TOLERANCE = "qcChannelsTolerance";

const QString Settings::OPT_CLASSIFIER_ENABLED = "classifier%1Enabled";

const QString Settings::OPT_CLASSIFIERS_CONFIG = "classifiersConfig";

Settings* Settings::instance;

Settings::Settings(QString path) :
    QSettings(path, QSettings::NativeFormat)
{
}

void Settings::instantiate(QString path){
    Settings::instance = new Settings(path);
}

Settings* Settings::getInstance(){
    return Settings::instance;
}

void Settings::updateValue(const QString & key, const QVariant & value){
    this->setValue(key, value);
    qDebug()<<"Updating config value";
    emit configurationChanged();
    emit configurationChanged(key);
}

bool Settings::isDummyModeEnabled(){
    QString reply = getInstance()->value(Settings::OPT_DUMMY_DAQ, QString("true")).toString();
    return reply.compare("true", Qt::CaseInsensitive)==0;
}

void Settings::setDummyDaqEnabled(bool enabled){
    getInstance()->updateValue(Settings::OPT_DUMMY_DAQ, enabled);
}

bool Settings::isDummyDaqNoisy(){
    QString reply = getInstance()->value(Settings::OPT_DUMMY_DAQ_NOISY, QString("true")).toString();
    return reply.compare("true", Qt::CaseInsensitive)==0;
}

void Settings::setDummyDaqNoisy(bool noisy){
    getInstance()->updateValue(Settings::OPT_DUMMY_DAQ_NOISY, noisy);
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

QString Settings::octaveScriptsRoot(){
    return getInstance()->value(Settings::OPT_OCTAVE_SCRIPTS_ROOT, "./scripts").toString();
}

void Settings::setOctaveScriptsRoot(QString value){
    getInstance()->updateValue(Settings::OPT_OCTAVE_SCRIPTS_ROOT, value);
}

void Settings::setSpellerPhrase(QString phrase){
    getInstance()->setValue(Settings::OPT_SPELLER_PHRASE, phrase);
}

int Settings::getDecimationFactcor(){
    return getInstance()->value(Settings::OPT_TRAIN_DECIMATION_FACTOR, "8").toInt();
}

int Settings::crossValidationRounds(){
    return getInstance()->value(Settings::OPT_TRAIN_XV_RATE, "5").toInt();
}

void Settings::setCrossvalidationRounds(int count){
    getInstance()->setValue(Settings::OPT_TRAIN_XV_RATE, count);
}

int Settings::periodOversampling(){
    return getInstance()->value(Settings::OPT_TRAIN_PERIOD_OVERSAMPLING, "3").toInt();
}

void Settings::setPeriodOversampling(int factor){
    getInstance()->setValue(Settings::OPT_TRAIN_PERIOD_OVERSAMPLING, factor);
}

QString Settings::useBalancing(){
    return getInstance()->value(Settings::OPT_TRAIN_BALANCING, "no").toString();
}

float Settings::getConfidenceThreshold(){
    return getInstance()->value(Settings::OPT_ONLINE_CONFIDENCE, "0.5").toFloat();
}

void Settings::setConfidenceThreshold(float value){
    getInstance()->setValue(Settings::OPT_ONLINE_CONFIDENCE, value);
}

int Settings::getOnlineMinEpochs(){
    return getInstance()->value(Settings::OPT_ONLINE_MIN_EPOCHS, "5").toInt();
}

void Settings::setOnlineMinEpochs(int value){
    getInstance()->setValue(Settings::OPT_ONLINE_MIN_EPOCHS, value);
}

bool Settings::qcInterrupt(){
    return getInstance()->value(Settings::OPT_QC_INTERRUPT, "false").toBool();
}

void Settings::setQcInterrupt(bool value){
    getInstance()->updateValue(Settings::OPT_QC_INTERRUPT, value);
}

double Settings::qcGoodnessLevel(){
    return getInstance()->value(Settings::OPT_QC_GOODNESS, "0.8").toDouble();
}

void Settings::setQcGoodnessLevel(double value){
    getInstance()->setValue(Settings::OPT_QC_GOODNESS, value);
}

int Settings::qcStrictness(){
    return getInstance()->value(Settings::OPT_QC_STRICTNESS, "10").toInt();
}

void Settings::setQcStrictness(int value){
    getInstance()->setValue(Settings::OPT_QC_STRICTNESS, value);
}

int Settings::qcChannelsTolerance(){
    return getInstance()->value(Settings::OPT_QC_CHANNELS_TOLERANCE, "4").toInt();
}

void Settings::setQcChannelsTolerance(int channels){
    getInstance()->setValue(Settings::OPT_QC_CHANNELS_TOLERANCE, channels);
}

void Settings::enableClassifier(QString name, bool enable){
    QString option = QString(OPT_CLASSIFIER_ENABLED).arg(name);
    getInstance()->setValue(option, enable);
}

bool Settings::classifierEnabled(QString name){
    QString option = QString(OPT_CLASSIFIER_ENABLED).arg(name);
    return getInstance()->value(option, "false").toBool();
}

QString Settings::classifiersConfig(){
    return getInstance()->value(Settings::OPT_CLASSIFIERS_CONFIG, "fastest").toString();
}

void Settings::setClassifiersConfig(QString path){
    getInstance()->setValue(Settings::OPT_CLASSIFIERS_CONFIG, path);
}
