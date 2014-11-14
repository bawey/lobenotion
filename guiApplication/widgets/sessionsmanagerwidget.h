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

class SessionsManagerWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit SessionsManagerWidget(QAbstractTableModel* model, QWidget *parent = 0);

signals:

public slots:
    void slotLoadPressed();
    void slotTrainPressed();
    void slotTestPressed();
    void slotDropPressed();

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
