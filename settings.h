#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

class Settings : public QSettings
{
    Q_OBJECT
public:
    static bool isDummyModeEnabled();
    static void setDummyModeEnabled(bool isIt);

    static bool isEegVisualizerEnabled();
    static void setEegVisualizerEnabled(bool isIt);

    static bool isTrainingMode();
    static void setTrainingMode(bool trueFalse);

    static int getSpellerHighlightFrames();
    static int getSpellerIntervalFrames();
    static int getSpellerHintFrames();
    static int getSpellerEpochPeriods();

    static QString getEegDumpPath();
    static QString getMetaDumpPath();

private:
    static QString OPT_DUMMY_DAQ;
    static QString OPT_EEG_VISUALIZER;
    static QString OPT_EEG_DUMP_PATH;
    static QString OPT_META_DUMP_PATH;

    static Settings* getInstance();
    static Settings* instance;
    explicit Settings();
    Settings(Settings &);
    
signals:
    
public slots:
    
};

#endif // SETTINGS_H
