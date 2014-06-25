#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
    Q_OBJECT
public:
    static QString OPT_DUMMY_DAQ;
    static QString OPT_EEG_VISUALIZER;

    static bool isDummyModeEnabled();
    static void setDummyModeEnabled(bool isIt);

    static bool isEegVisualizerEnabled();
    static void setEegVisualizerEnabled(bool isIt);

    static bool isTrainingMode();
    static void setTrainingMode(boolean trueFalse);

    static int getSpellerHighlightTicks();
    static int getSpellerIntervalTicks();
    static int getSpellerMessageTicks();
    static int getSpellerHighlightIterations();

private:
    static Settings* getInstance();
    static Settings* instance;
    explicit Settings();
    Settings(Settings &);
    
signals:
    
public slots:
    
};

#endif // SETTINGS_H
