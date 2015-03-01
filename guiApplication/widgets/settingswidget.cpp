#include "settingswidget.h"
#include "settings.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    this->valueChannelsTolerance->setRange(0, 12);
    this->valueGoodness->setRange(0, 1);
//    this->valueStrictness->setRange(0, 100);

    layout->addRow(labelInterrupt, valueInterrupt);
    layout->addRow(labelGoodness, valueGoodness);
    layout->addRow(labelChannelsTolerance, valueChannelsTolerance);
//    layout->addRow(labelStrictness, valueStrictness);

    buttonOctave->setMinimumWidth(1);
    buttonOctave->setText("Browse");
    rowOctave -> addWidget(valueOctaveDir);
    rowOctave -> addWidget(buttonOctave);
    layout->addRow(labelOctaveRoot, rowOctave);
    dirMapper->setMapping(buttonOctave, valueOctaveDir);

    buttonDataDir->setMinimumWidth(1);
    buttonDataDir->setText("Browse");
    rowDataDir -> addWidget(valueDataDir);
    rowDataDir -> addWidget(buttonDataDir);
    layout->addRow(labelDataDir, rowDataDir);
    dirMapper->setMapping(buttonDataDir, valueDataDir);

    this->setLayout(this->layout);
    this->connectSignalsSlots();
}

void SettingsWidget::connectSignalsSlots(){
    connect(valueInterrupt, SIGNAL(clicked(bool)), Settings::getInstance(), SLOT(setQcInterrupt(bool)));
    connect(valueChannelsTolerance, SIGNAL(valueChanged(int)), Settings::getInstance(), SLOT(setQcChannelsTolerance(int)));
    connect(valueGoodness, SIGNAL(valueChanged(double)), Settings::getInstance(), SLOT(setQcGoodnessLevel(double)));
//    connect(valueStrictness, SIGNAL(valueChanged(int)), Settings::getInstance(), SLOT(setQcStrictness(int)));
    connect(valueOctaveDir, SIGNAL(textChanged(QString)), Settings::getInstance(), SLOT(setOctaveScriptsRoot(QString)));
    connect(valueDataDir, SIGNAL(textChanged(QString)), Settings::getInstance(), SLOT(setEegDumpPath(QString)));

    connect(buttonDataDir, SIGNAL(clicked()), dirMapper, SLOT(map()));
    connect(buttonOctave, SIGNAL(clicked()), dirMapper, SLOT(map()));
    connect(dirMapper, SIGNAL(mapped(QWidget*)), this, SLOT(pickDir(QWidget*)));
}

/**
 * @brief SettingsWidget::revalidate updates display with current values of parameters
 */
void SettingsWidget::revalidate(){
    Settings* s = Settings::getInstance();

    valueGoodness->setValue(s->qcGoodnessLevel());
    valueInterrupt->setChecked(s->qcInterrupt());
    valueChannelsTolerance->setValue(s->qcChannelsTolerance());
//    valueStrictness->setValue(s->qcStrictness());
    valueOctaveDir->setText(s->octaveScriptsRoot());
    valueDataDir->setText(s->getEegDumpPath());
}

void SettingsWidget::pickDir(QWidget* field){
    ((QLineEdit*)field)->setText(QFileDialog::getExistingDirectory());
}
