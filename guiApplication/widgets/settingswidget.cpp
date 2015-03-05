#include "settingswidget.h"
#include "settings.h"
#include <QFileInfo>
#include <QDebug>
#include <QUrl>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QScrollArea(parent)
{
    layout->addRow(labelDummyDaq, valueDummyDaq);
    layout->addRow(labelDummyNoise, valueDummyNoise);

    this->valueChannelsTolerance->setRange(0, 12);
    this->valueGoodness->setRange(0, 1);
    valueGoodness->setSingleStep(0.01);
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

    buttonClassCnf->setText("Browse");
    rowClassCnf->addWidget(valueClassCnf);
    rowClassCnf->addWidget(buttonClassCnf);
    layout->addRow(labelClassifierCnf, rowClassCnf);

    valueOversampling->setRange(1,12);
    layout->addRow(labelPeriodOversampling, valueOversampling);

    valueXvFold->setRange(1,100);
    layout->addRow(labelXvFold, valueXvFold);

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
    connect(valueClassCnf, SIGNAL(textChanged(QString)), Settings::getInstance(), SLOT(setClassifiersConfig(QString)));
    connect(buttonClassCnf, SIGNAL(clicked()), this, SLOT(pickClassifiersConfig()));

    connect(valueXvFold, SIGNAL(valueChanged(int)), Settings::getInstance(), SLOT(setCrossvalidationRounds(int)));
    connect(valueOversampling, SIGNAL(valueChanged(int)), Settings::getInstance(), SLOT(setPeriodOversampling(int)));
    connect(valueDummyDaq, SIGNAL(clicked(bool)), Settings::getInstance(), SLOT(setDummyDaqEnabled(bool)));
    connect(valueDummyNoise, SIGNAL(clicked(bool)), Settings::getInstance(), SLOT(setDummyDaqNoisy(bool)));
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
    valueClassCnf->setText(s->classifiersConfig());
    valueOversampling->setValue(s->periodOversampling());
    valueXvFold->setValue(s->crossValidationRounds());
    valueDummyDaq->setChecked(s->isDummyModeEnabled());
    valueDummyNoise->setChecked(s->isDummyDaqNoisy());
}

void SettingsWidget::pickDir(QWidget* field){
    QString txt = QFileDialog::getExistingDirectory();
    if(QFileInfo(txt).exists()){
        ((QLineEdit*)field)->setText(txt);
    }
}

void SettingsWidget::pickClassifiersConfig(){
    QFileInfo currentFileInfo = QFileInfo(Settings::classifiersConfig());
    QString url;
    if(currentFileInfo.exists()){
        url = currentFileInfo.dir().absolutePath();
    }else if(QFileInfo(Settings::octaveScriptsRoot()).exists()){
        url = Settings::octaveScriptsRoot();
    }else{
        url="~";
    }
    QString chosen = QFileDialog::getOpenFileName(this, "caption", url, "*.cnf");
    qDebug()<<"cnf path: "<<chosen;
    if(QFileInfo(chosen).exists()){
        valueClassCnf->setText(chosen);
    }
}

