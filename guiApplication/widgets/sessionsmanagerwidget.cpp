#include "sessionsmanagerwidget.h"
#include <QDebug>
#include <QFileDialog>
#include <settings.h>
#include <sessionsmodel.h>
#include <QHeaderView>
#include <QModelIndexList>

SessionsManagerWidget::SessionsManagerWidget(QAbstractTableModel *model, QWidget *parent) :
    QGroupBox("Loaded sessions", parent)
{
    tableView = new QTableView();
    tableView->setModel(model);

    topLayout = new QVBoxLayout();
    buttonsLayout = new QHBoxLayout();
    buttonsGroup = new QButtonGroup();

    buttonLoad=new QPushButton("Load");
    buttonTrain=new QPushButton("Train on selected");
    buttonTest=new QPushButton("Test on selected");
    buttonDrop=new QPushButton("Drop selected");


    topLayout->addWidget(tableView);
    tableView->show();
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int c = 0; c < tableView->horizontalHeader()->count(); ++c)
    {
        tableView->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }


    buttonsGroup->addButton(buttonLoad);
    buttonsGroup->addButton(buttonTrain);
    buttonsGroup->addButton(buttonTest);

    buttonsLayout->addWidget(buttonLoad);
    buttonsLayout->addWidget(buttonTrain);
    buttonsLayout->addWidget(buttonTest);
    buttonsLayout->addWidget(buttonDrop);
    topLayout->addLayout(buttonsLayout);

    this->setLayout(topLayout);
    connectInternalSignals();
}

void SessionsManagerWidget::connectInternalSignals(){
    connect(buttonLoad, SIGNAL(clicked()), this, SLOT(slotLoadPressed()));
    connect(buttonTrain, SIGNAL(clicked()), this, SLOT(slotTrainPressed()));
    connect(buttonTest, SIGNAL(clicked()), this, SLOT(slotTestPressed()));
    connect(buttonDrop, SIGNAL(clicked()), this, SLOT(slotDropPressed()));

    //finish tomorrow - enable, disable buttons depending on that...
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection))

    qDebug()<<"For now, test button will populate the model with some dummy data";

}

void SessionsManagerWidget::slotLoadPressed(){

    QString dirpath = Settings::getEegDumpPath();
    QStringList filenames = QFileDialog::getOpenFileNames(0, "Pick a session summary file", dirpath, "Lobenotion dump summaries (*_summary)");
    QStringList uniqueNameRoots;
    foreach (QString filename, filenames) {
        filename = filename.mid(0,filename.lastIndexOf('_'));
        if(!uniqueNameRoots.contains(filename)){
            uniqueNameRoots.append(filename);
        }
    }

    foreach (QString filename, uniqueNameRoots) {
        qDebug()<<"chosen: "<<filename<<"from dir: "<<dirpath;
    }
    ((SessionsModel*) tableView->model())->slotLoadSession(uniqueNameRoots);
}

void SessionsManagerWidget::slotTrainPressed(){
    QModelIndexList selectionList = tableView->selectionModel()->selectedRows();
}

void SessionsManagerWidget::slotTestPressed(){
    QModelIndexList selectionList = tableView->selectionModel()->selectedRows();
}

void SessionsManagerWidget::slotDropPressed(){
    QModelIndexList selectionList = tableView->selectionModel()->selectedRows();
}
