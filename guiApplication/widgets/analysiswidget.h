#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "sessionsmanagerwidget.h"
#include "classifiersmanagerwidget.h"
#include "octaveoutputwidget.h"
#include "master.h"
#include "classifierinfo.h"
#include "octaveEmbedded/p3sessioninfo.h"

class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisWidget(QWidget *parent = 0);

    const SessionsManagerWidget * const sessionsManagerWidget() const {
        return this->sessionsWidget;
    }

    const OctaveOutputWidget* const octaveOutputWidget() const {
        return this->octaveWidget;
    }

    const ClassifiersManagerWidget* const classifiersManagerWidget() const {
        return this->classifiersWidget;
    }

private:
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* sessionsModelsLayout = new QHBoxLayout();
    SessionsManagerWidget* sessionsWidget = new SessionsManagerWidget(Master::getInstance()->getSessionsModel());
    OctaveOutputWidget* octaveWidget = new OctaveOutputWidget();
    ClassifiersManagerWidget* classifiersWidget = new ClassifiersManagerWidget();


signals:
//    void signalAnalyzeConfidence(const ClassifierInfo* const classfier, QList<const P3SessionInfo* const>* data);
public slots:
    void slotAnalyzeConfidence() const;
};

#endif // ANALYSISWIDGET_H
