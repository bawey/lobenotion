#ifndef SESSIONSMANAGERWIDGET_H
#define SESSIONSMANAGERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QAbstractTableModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QList>
#include <p3sessioninfo.h>
#include <QSharedPointer>

class SessionsManagerWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit SessionsManagerWidget(QAbstractTableModel* model, QWidget *parent = 0);
    QSharedPointer<QList<const P3SessionInfo*>> selectedSessions() const;

signals:

    void signalTestModel(QSharedPointer<QList<const P3SessionInfo*>>);

public slots:
    void enableButtons(bool enable);
    void slotLoadPressed();
    void slotTrainPressed();
    void slotTestPressed();
    void slotDropPressed();

private slots:
    void slotAdaptButtonsStateToSelection();

private:
   QVBoxLayout* topLayout;
   QHBoxLayout* buttonsLayout;

   QButtonGroup* buttonsGroup;
   QPushButton* buttonLoad;
   QPushButton* buttonTrain;
   QPushButton* buttonTest;
   QPushButton* buttonDrop;

   QTableView* tableView;


   void connectInternalSignals();
};

#endif // SESSIONSMANAGERWIDGET_H
