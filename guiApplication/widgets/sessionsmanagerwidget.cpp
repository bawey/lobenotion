#include "sessionsmanagerwidget.h"
#include <QDebug>
#include <QFileDialog>
#include <settings.h>
#include <sessionsmodel.h>
#include <QHeaderView>
#include <QModelIndexList>
#include <QtConcurrent/QtConcurrent>
#include <master.h>
#include <QSharedPointer>

SessionsManagerWidget::SessionsManagerWidget(QAbstractTableModel *model, QWidget *parent) :
    QGroupBox("Loaded sessions", parent)
{
    tableView = new QTableView();
    tableView->setModel(model);

    topLayout = new QVBoxLayout();
    buttonsLayout = new QHBoxLayout();
    buttonsGroup = new QButtonGroup();

    buttonLoad=new QPushButton("Load");
    buttonTrain=new QPushButton("Train");
    buttonTest=new QPushButton("Test");
    buttonDrop=new QPushButton("Drop");


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
    this->slotAdaptButtonsStateToSelection();
    qRegisterMetaType<QSharedPointer<QList<const P3SessionInfo*>>>("QSharedPointer<QList<const P3SessionInfo*>>");
}

void SessionsManagerWidget::connectInternalSignals(){
    connect(buttonLoad, SIGNAL(clicked()), this, SLOT(slotLoadPressed()));
    connect(buttonTrain, SIGNAL(clicked()), this, SLOT(slotTrainPressed()));
    connect(buttonTest, SIGNAL(clicked()), this, SLOT(slotTestPressed()));
    connect(buttonDrop, SIGNAL(clicked()), this, SLOT(slotDropPressed()));

    //finish tomorrow - enable, disable buttons depending on that...
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(slotAdaptButtonsStateToSelection()));

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
    QtConcurrent::run((SessionsModel*) tableView->model(), &SessionsModel::slotLoadSession, uniqueNameRoots);
}

void SessionsManagerWidget::slotTrainPressed(){
    //TODO: move into some private helper method
    QModelIndexList selectionList = tableView->selectionModel()->selectedRows();
    QList<unsigned short> positions;
    foreach(QModelIndex index, selectionList){
        positions.append((unsigned short)index.row());
    }
    QList<const P3SessionInfo*>* sessionsInfo = ((SessionsModel*)tableView->model())->getSessionsAt(positions);
    QSharedPointer<QList<const P3SessionInfo*>> sessionsSharedInfo = QSharedPointer<QList<const P3SessionInfo*>>(sessionsInfo);
    //EOTODO


    QtConcurrent::run(Master::getInstance()->getClassifiersModel(), &ClassifiersModel::slotTrainModel, sessionsSharedInfo);
}

void SessionsManagerWidget::slotTestPressed(){
    //TODO: move into some private helper method
    QModelIndexList selectionList = tableView->selectionModel()->selectedRows();
    QList<unsigned short> positions;
    foreach(QModelIndex index, selectionList){
        positions.append((unsigned short)index.row());
    }
    QList<const P3SessionInfo*>* sessionsInfo = ((SessionsModel*)tableView->model())->getSessionsAt(positions);
    QSharedPointer<QList<const P3SessionInfo*>> sessionsSharedInfo = QSharedPointer<QList<const P3SessionInfo*>>(sessionsInfo);
    //EOTODO
    emit signalTestModel(sessionsSharedInfo);

}

void SessionsManagerWidget::slotDropPressed(){

}

void SessionsManagerWidget::slotAdaptButtonsStateToSelection(){

    QModelIndexList selectedRows = tableView->selectionModel()->selectedRows();

    foreach(QModelIndex index, selectedRows){
        qDebug()<<"selected row: "<<index.row();
    }

    bool anyselected = selectedRows.length()>0;
    buttonTrain->setEnabled(anyselected);
    buttonTest->setEnabled(anyselected);
    buttonDrop->setEnabled(anyselected);

}
