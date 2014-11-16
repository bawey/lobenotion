#include "classifiersmanagerwidget.h"
#include <master.h>
#include <QHeaderView>

ClassifiersManagerWidget::ClassifiersManagerWidget(QWidget *parent) :
    QGroupBox("Trained models",parent)
{
    tableView = new QTableView();
    tableView->setModel(Master::getInstance()->getClassifiersModel());
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int c = 0; c < tableView->horizontalHeader()->count(); ++c)
    {
        tableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    buttonDrop = new QPushButton("Drop");

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(buttonDrop);
    this->setLayout(mainLayout);

    connectInternalSignals();
    slotAdaptButtonsStateToSelection();
}

void ClassifiersManagerWidget::connectInternalSignals(){
    connect(buttonDrop, SIGNAL(clicked()), this, SLOT(slotButtonDropPressed()));
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(slotAdaptButtonsStateToSelection()));
}

void ClassifiersManagerWidget::slotButtonDropPressed(){

}

void ClassifiersManagerWidget::slotAdaptButtonsStateToSelection(){
    buttonDrop->setEnabled(tableView->selectionModel()->selectedRows().length()>0);
}
