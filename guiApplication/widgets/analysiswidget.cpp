#include "analysiswidget.h"
#include "master.h"
#include <QList>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QSharedPointer>

AnalysisWidget::AnalysisWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(mainLayout);
    sessionsModelsLayout->addWidget(sessionsWidget);
    sessionsModelsLayout->addWidget(classifiersWidget);
    mainLayout->addLayout(sessionsModelsLayout);
    mainLayout->addWidget(octaveWidget);

    connect(classifiersWidget, SIGNAL(signalAnalyzeConfidence()), this, SLOT(slotAnalyzeConfidence()));
}

void AnalysisWidget::slotAnalyzeConfidence() const{
    const ClassifierInfo* model = classifiersWidget->selectedClassifier();
    QtConcurrent::run(Master::getInstance()->getOctaveProxy(), &OctaveProxy::slotAnalyzeConfidence, model,
        sessionsManagerWidget()->selectedSessions());
}
