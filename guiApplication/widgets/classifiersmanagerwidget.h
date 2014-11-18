#ifndef CLASSIFIERSMANAGERWIDGET_H
#define CLASSIFIERSMANAGERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSharedPointer>
#include <octaveEmbedded/p3sessioninfo.h>
#include <QList>




class ClassifiersManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit ClassifiersManagerWidget(QWidget *parent = 0);

signals:
    void signalGoOnline();
public slots:
    void slotTakeSessionsForTest(QSharedPointer<QList<const P3SessionInfo*>>);
private slots:
    void slotButtonDropPressed();
    void slotButtonOnlinePressed();
    void slotAdaptButtonsStateToSelection();

private:
    QVBoxLayout* mainLayout;
    QTableView* tableView;
    QPushButton* buttonDrop;
    QPushButton* buttonOnline;

    void connectInternalSignals();
    bool ensureOneSelected();
};

#endif // CLASSIFIERSMANAGERWIDGET_H
