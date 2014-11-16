#ifndef CLASSIFIERSMANAGERWIDGET_H
#define CLASSIFIERSMANAGERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>




class ClassifiersManagerWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit ClassifiersManagerWidget(QWidget *parent = 0);

signals:

public slots:

private slots:
    void slotButtonDropPressed();
    void slotAdaptButtonsStateToSelection();
private:
    QVBoxLayout* mainLayout;
    QTableView* tableView;
    QPushButton* buttonDrop;

    void connectInternalSignals();
};

#endif // CLASSIFIERSMANAGERWIDGET_H
