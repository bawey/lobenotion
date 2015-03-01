#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

class Settings : public QSettings
{
    Q_OBJECT
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

    static int getTrainCvRate();

    static int getTrainPeriodSplitRate();

    static QString getTrainBalancing();

    static float getConfidenceThreshold();

    static int getOnlineMinEpochs();

    static bool qcInterrupt();
    static double qcGoodnessLevel();
    static int qcStrictness();
    static int qcChannelsTolerance();

private:
    static QString DEF_SPELLER_CHARSET;

    static QString OPT_DUMMY_DAQ;
    static QString OPT_DUMMY_DAQ_NOISY;
    static QString OPT_EEG_VISUALIZER;
    static QString OPT_EEG_DUMP_PATH;
    static QString OPT_META_DUMP_PATH;
    static QString OPT_SPELLER_CHARSET;
    static QString OPT_SPELLER_MATRIX_SIZE;
    static QString OPT_SPELLER_HIGHLIGHT_STINT;
    static QString OPT_SPELLER_DIM_STINT;
    static QString OPT_SPELLER_INTER_PERIOD_STINT;
    static QString OPT_SPELLER_INFO_STINT;
    static QString OPT_SPELLER_EPOCHS_PER_STIMULUS;

    static QString OPT_SUBJECT_NAME;
    static QString OPT_SPELLER_PHRASE;
    static QString OPT_OCTAVE_SCRIPTS_ROOT;

    static QString OPT_TRAIN_DECIMATION_FACTOR;
    static QString OPT_TRAIN_XV_RATE;
    static QString OPT_TRAIN_PERIOD_SPLIT_RATE;
    static QString OPT_TRAIN_BALANCING;

    static QString OPT_ONLINE_CONFIDENCE;
    static QString OPT_ONLINE_MIN_EPOCHS;

    static QString OPT_QC_INTERRUPT;
    static QString OPT_QC_GOODNESS;
    static QString OPT_QC_STRICTNESS;
    static QString OPT_QC_CHANNELS_TOLERANCE;

    static Settings* instance;
    explicit Settings();
    Settings(Settings &);

    void updateValue(const QString & key, const QVariant & value);
    
signals:
    void configurationChanged();
    
public slots:
    static void setDummyModeEnabled(bool isIt);
    static void setEegVisualizerEnabled(bool isIt);
    static void setSpellerHighlightStint(int thestint);
    static void setSpellerDimStint(int thestint);
    static void setSpellerInfoStint(int thestint);
    static void setSpellerInterPeriodStint(int thestint);
    static void setSpellerEpochsPerStimulus(int epochs);
    static void setEegDumpPath(QString newPath);
    static void setSubjectName(QString name);
    static void setSpellerPhrase(QString phrase);
    static void setConfidenceThreshold(float value);
    static void setOnlineMinEpochs(int value);
    static void setQcInterrupt(bool);
    static void setQcGoodnessLevel(double);
    static void setQcStrictness(int);
    static void setQcChannelsTolerance(int);
    static void setOctaveScriptsRoot(QString value);
};

#endif // SETTINGS_H
