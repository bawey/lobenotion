#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QScrollArea>
#include "classifierswidget.h"

class SettingsWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = 0);

signals:

public slots:
    void revalidate();

private slots:
    void pickDir(QWidget*);
    void pickClassifiersConfig();

private:
    QSignalMapper *dirMapper = new QSignalMapper();
    QFormLayout *layout = new QFormLayout();
    QLabel *labelInterrupt = new QLabel("Interrupt on poor signal?");
    QLabel *labelChannelsTolerance = new QLabel("Tolerable poor channels limit");
    QLabel *labelGoodness = new QLabel("Channel quality required to be marked as good");
//    QLabel *labelStrictness = new QLabel("Quality control strictness");
    QLabel *labelOctaveRoot = new QLabel("Octave scripts directory");
    QLabel *labelDataDir = new QLabel("Data directory");
    QLabel *labelClassifiers = new QLabel("Classifiers for training");
    QLabel *labelClassifierCnf = new QLabel("Classifiers config");
    QLabel *labelPeriodOversampling = new QLabel("Period oversampling");
    QLabel *labelXvFold = new QLabel("Cross-validation rounds");


    QCheckBox *valueInterrupt = new QCheckBox(this);
    QSpinBox *valueChannelsTolerance = new QSpinBox(this);
    QDoubleSpinBox *valueGoodness = new QDoubleSpinBox(this);
    QSpinBox *valueOversampling = new QSpinBox(this);
    QSpinBox *valueXvFold = new QSpinBox(this);
//    QSpinBox *valueStrictness = new QSpinBox(this);

    QLineEdit *valueOctaveDir = new QLineEdit(this);
    QHBoxLayout *rowOctave = new QHBoxLayout();
    QPushButton *buttonOctave = new QPushButton(this);

    QLineEdit *valueDataDir = new QLineEdit(this);
    QHBoxLayout *rowDataDir = new QHBoxLayout();
    QPushButton *buttonDataDir = new QPushButton(this);

    QLineEdit *valueClassCnf = new QLineEdit(this);
    QHBoxLayout *rowClassCnf = new QHBoxLayout();
    QPushButton *buttonClassCnf = new QPushButton(this);

    void connectSignalsSlots();
};

#endif // SETTINGSWIDGET_H
