#ifndef CLASSIFIERSMANAGERWIDGET_H
#define CLASSIFIERSMANAGERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSharedPointer>
#include <p3sessioninfo.h>
#include <classifierinfo.h>
#include <QList>
#include "classifiersmodel.h"




class ClassifiersManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit ClassifiersManagerWidget(QWidget *parent = 0);
    ClassifierInfo * selectedClassifier() const;

signals:
    void signalGoOnline();
    void signalAnalyzeConfidence();
public slots:
    void slotTakeSessionsForTest(QSharedPointer<QList<const P3SessionInfo*>>);
    void enableButtons(bool enable);
private slots:
    void slotButtonDropPressed();
    void slotButtonOnlinePressed();
    void slotAdaptButtonsStateToSelection();
    void slotAnalyzeConfidencePressed();

private:
    ClassifiersModel* model;
    QVBoxLayout* mainLayout;
    QTableView* tableView;
    QPushButton* buttonDrop;
    QPushButton* buttonOnline;
    QPushButton* buttonConfidence;

    void connectInternalSignals();
    bool ensureOneSelected();
};

#endif // CLASSIFIERSMANAGERWIDGET_H
