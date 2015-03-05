#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QList>
#include <QSet>
#include <master.h>

class Settings : public QSettings
{
    Q_OBJECT
    friend class Master;
public:
    static Settings* getInstance();

    static bool isDummyModeEnabled();
    static bool isDummyDaqNoisy();
    static bool isEegVisualizerEnabled();

    static bool isTrainingMode();

    static int getSpellerHighlightStint();

    static int getSpellerDimStint();

    static int getSpellerInfoStint();

    static int getSpellerInterPeriodStint();

    static int getSpellerEpochsPerStimulus();

    static QString getEegDumpPath();

    static QString getSubjectName();

    static QString getSpellerPhrase();

    //these can for now only be set in the file and taken into account upon restarting the program
    static QString getMetaDumpPath(); // Deprecated, I think
    static QString getSpellerCharset();
    static unsigned short getSpellerMatrixSize();
    static QString octaveScriptsRoot();


    static int getDecimationFactcor();
    static QString useBalancing();

    static float getConfidenceThreshold();

    static int getOnlineMinEpochs();

    static bool qcInterrupt();
    static double qcGoodnessLevel();
    static int qcStrictness();
    static int qcChannelsTolerance();

    static bool classifierEnabled(QString name);
    static QSet<QString>* runtimeClassifiers(){ return getInstance()->runtimeClassifiersSet; }

    static int periodOversampling();
    static int crossValidationRounds();

    static QString classifiersConfig();

public:
    static const QString DEF_SPELLER_CHARSET;

    static const QString OPT_DUMMY_DAQ;
    static const QString OPT_DUMMY_DAQ_NOISY;
    static const QString OPT_EEG_VISUALIZER;
    static const QString OPT_EEG_DUMP_PATH;
    static const QString OPT_META_DUMP_PATH;
    static const QString OPT_SPELLER_CHARSET;
    static const QString OPT_SPELLER_MATRIX_SIZE;
    static const QString OPT_SPELLER_HIGHLIGHT_STINT;
    static const QString OPT_SPELLER_DIM_STINT;
    static const QString OPT_SPELLER_INTER_PERIOD_STINT;
    static const QString OPT_SPELLER_INFO_STINT;
    static const QString OPT_SPELLER_EPOCHS_PER_STIMULUS;

    static const QString OPT_SUBJECT_NAME;
    static const QString OPT_SPELLER_PHRASE;
    static const QString OPT_OCTAVE_SCRIPTS_ROOT;

    static const QString OPT_TRAIN_DECIMATION_FACTOR;
    static const QString OPT_TRAIN_XV_RATE;
    static const QString OPT_TRAIN_PERIOD_OVERSAMPLING;
    static const QString OPT_TRAIN_BALANCING;
    static const QString OPT_CLASSIFIERS_CONFIG;

    static const QString OPT_ONLINE_CONFIDENCE;
    static const QString OPT_ONLINE_MIN_EPOCHS;

    static const QString OPT_QC_INTERRUPT;
    static const QString OPT_QC_GOODNESS;
    static const QString OPT_QC_STRICTNESS;
    static const QString OPT_QC_CHANNELS_TOLERANCE;

    static const QString OPT_CLASSIFIER_ENABLED;


private:
    static Settings* instance;
    static void instantiate(QString path);
    explicit Settings(QString path);
    Settings(Settings &);

    void updateValue(const QString & key, const QVariant & value);
    QSet<QString>* runtimeClassifiersSet = new QSet<QString>();
    
signals:
    void configurationChanged();
    void configurationChanged(QString paraname);
    
public slots:
    static void setEegVisualizerEnabled(bool isIt);
    static void setSpellerHighlightStint(int thestint);
    static void setSpellerDimStint(int thestint);
    static void setSpellerInfoStint(int thestint);
    static void setSpellerInterPeriodStint(int thestint);
    static void setSpellerEpochsPerStimulus(int epochs);
    static void setEegDumpPath(QString newPath);
    static void setSubjectName(QString name);
    static void setSpellerPhrase(QString phrase);
    static void setConfidenceThreshold(double value);
    static void setOnlineMinEpochs(int value);
    static void setQcInterrupt(bool);
    static void setQcGoodnessLevel(double);
    static void setQcStrictness(int);
    static void setQcChannelsTolerance(int);
    static void setOctaveScriptsRoot(QString value);
    static void enableClassifier(QString name, bool enable);
    static void addRuntimeClassifier(QString name){ getInstance()->runtimeClassifiersSet->insert(name); }
    static void setClassifiersConfig(QString path);
    static void setPeriodOversampling(int factor);
    static void setCrossvalidationRounds(int count);
    static void setDummyDaqNoisy(bool noisy);
    static void setDummyDaqEnabled(bool enabled);
};

#endif // SETTINGS_H
