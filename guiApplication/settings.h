#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

class Settings : public QSettings
{
    Q_OBJECT
public:
    static bool isDummyModeEnabled();
    static bool isDummyDaqNoisy();

    static void setDummyModeEnabled(bool isIt);

    static bool isEegVisualizerEnabled();
    static void setEegVisualizerEnabled(bool isIt);

    static bool isTrainingMode();
    static void setTrainingMode(bool trueFalse);

    static int getSpellerHighlightStint();
    static void setSpellerHighlightStint(int thestint);

    static int getSpellerDimStint();
    static void setSpellerDimStint(int thestint);

    static int getSpellerInfoStint();
    static void setSpellerInfoStint(int thestint);

    static int getSpellerInterPeriodStint();
    static void setSpellerInterPeriodStint(int thestint);

    static int getSpellerEpochsPerStimulus();
    static void setSpellerEpochsPerStimulus(int epochs);

    static QString getEegDumpPath();
    static void setEegDumpPath(QString newPath);

    static QString getSubjectName();
    static void setSubjectName(QString name);

    static QString getSpellerPhrase();
    static void setSpellerPhrase(QString phrase);

    //these can for now only be set in the file and taken into account upon restarting the program
    static QString getMetaDumpPath(); // Deprecated, I think
    static QString getSpellerCharset();
    static unsigned short getSpellerMatrixSize();
    static QString getOctaveScriptsRoot();


    static int getDecimationFactcor();

    static int getTrainCvRate();

    static int getTrainPeriodSplitRate();

    static QString getTrainBalancing();

    static float getConfidenceThreshold();
    static void setConfidenceThreshold(float value);

    static int getOnlineMinEpochs();
    static void setOnlineMinEpochs(int value);

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

    static Settings* getInstance();
    static Settings* instance;
    explicit Settings();
    Settings(Settings &);
    
signals:
    
public slots:
    
};

#endif // SETTINGS_H
