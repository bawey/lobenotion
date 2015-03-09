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
    QSharedPointer<QList<const P3SessionInfo*>> sessionsInfo = selectedSessions();

    QtConcurrent::run(Master::getInstance()->getClassifiersModel(), &ClassifiersModel::slotTrainModel, sessionsInfo);
}

void SessionsManagerWidget::slotTestPressed(){
    QSharedPointer<QList<const P3SessionInfo*>> sessionsSharedInfo = selectedSessions();
    emit signalTestModel(sessionsSharedInfo);
}

void SessionsManagerWidget::slotDropPressed(){
    QModelIndexList selectedRows = tableView->selectionModel()->selectedRows();

    while(!selectedRows.isEmpty()){
        QModelIndex index = selectedRows.takeLast();
        qDebug()<<"Removing session at "<<index.row();
        tableView->model()->removeRow(index.row());
    }
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

QSharedPointer<QList<const P3SessionInfo*>> SessionsManagerWidget::selectedSessions() const {
    QModelIndexList selectionList = tableView->selectionModel()->selectedRows();
    QList<unsigned short> positions;
    foreach(QModelIndex index, selectionList){
        positions.append((unsigned short)index.row());
    }
    QSharedPointer<QList<const P3SessionInfo*>> sessionInfos = ((SessionsModel*)tableView->model())->getSessionsAt(positions);
    return sessionInfos;
}

void SessionsManagerWidget::adaptButtonsToOctaveState(bool enable){
    tableView->setEnabled(enable);
    buttonLoad->setEnabled(enable);
    if(enable){
        slotAdaptButtonsStateToSelection();
    }else{
        buttonDrop->setEnabled(false);
        buttonTest->setEnabled(false);
        buttonTrain->setEnabled(false);                                        
    }
}
