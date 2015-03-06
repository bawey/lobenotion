#include "classifiersmanagerwidget.h"
#include <master.h>
#include <QHeaderView>
#include <octaveproxy.h>
#include <QtConcurrent/QtConcurrent>
#include <QModelIndexList>
#include <QMessageBox>
#include <QHBoxLayout>

ClassifiersManagerWidget::ClassifiersManagerWidget(QWidget *parent) :
    QGroupBox("Trained models",parent)
{
    model = Master::getInstance()->getClassifiersModel();
    tableView = new QTableView();
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int c = 0; c < tableView->horizontalHeader()->count(); ++c)
    {
        QHeaderView::ResizeMode resizeMode = QHeaderView::ResizeToContents;
        if( c==tableView->horizontalHeader()->count()-1 ){
            resizeMode = QHeaderView::Stretch;
        }
        tableView->horizontalHeader()->setSectionResizeMode(c, resizeMode);
    }

    buttonDrop = new QPushButton("Drop");
//    buttonDrop->setSizePolicy(QSizePolicy::Fixed);
    buttonOnline = new QPushButton("Use online");
//    buttonOnline->setSizePolicy(QSizePolicy::Fixed);
    buttonConfidence = new QPushButton("Analyze confidence");

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tableView);
    QHBoxLayout* buttonsContainer = new QHBoxLayout();

    buttonsContainer->addWidget(buttonDrop);
    buttonsContainer->addWidget(buttonOnline);
    buttonsContainer->addWidget(buttonConfidence);

    mainLayout->addLayout(buttonsContainer);
    this->setLayout(mainLayout);

    connectInternalSignals();
    slotAdaptButtonsStateToSelection();
}

void ClassifiersManagerWidget::connectInternalSignals(){
    connect(buttonDrop, SIGNAL(clicked()), this, SLOT(slotButtonDropPressed()));
    connect(buttonOnline, SIGNAL(clicked()), this, SLOT(slotButtonOnlinePressed()));
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(slotAdaptButtonsStateToSelection()));
    connect(buttonConfidence, SIGNAL(clicked()), this, SLOT(slotAnalyzeConfidencePressed()));
}

void ClassifiersManagerWidget::slotButtonDropPressed(){

}

void ClassifiersManagerWidget::slotAdaptButtonsStateToSelection(){
    buttonDrop->setEnabled(tableView->selectionModel()->selectedRows().length()>0);
    buttonOnline->setEnabled(tableView->selectionModel()->selectedRows().length()>0);
    buttonConfidence->setEnabled(tableView->selectionModel()->selectedRows().length()>0);
}

void ClassifiersManagerWidget::slotTakeSessionsForTest(QSharedPointer<QList<const P3SessionInfo *> > sessions){
    ClassifiersModel* model = Master::getInstance()->getClassifiersModel();
    if(ensureOneSelected()){
        QModelIndexList indexList = tableView->selectionModel()->selectedRows();
        QtConcurrent::run(model, &ClassifiersModel::slotTestModel, (unsigned short)indexList.at(0).row(), sessions);
    }
}

bool ClassifiersManagerWidget::ensureOneSelected(){
    QModelIndexList indexList = tableView->selectionModel()->selectedRows();
    if(indexList.length()!=1){
        QMessageBox msgBox;
        if(indexList.length()==0){
            msgBox.setText("No classifier has been selected. Pick one and try again.");
        }else{
            msgBox.setText("Multiple classifiers have been selected. Pick one and try again.");
        }
        msgBox.exec();
        return false;
    }else{
        this->model->slotSetCurrentClassifier(indexList.at(0).row());
    }
    return true;
}

void ClassifiersManagerWidget::slotButtonOnlinePressed(){
    if(ensureOneSelected()){
        emit signalGoOnline();
        int selectedClassifier = tableView->selectionModel()->selectedRows().at(0).row();
        Master::getInstance()->getClassifiersModel()->slotSetCurrentClassifier(selectedClassifier);
    }
}

void ClassifiersManagerWidget::slotAnalyzeConfidencePressed(){
    if(ensureOneSelected()){
        emit signalAnalyzeConfidence();
    }
}

ClassifierInfo *ClassifiersManagerWidget::selectedClassifier() const {
    return model->chosenClassifier();
}

void ClassifiersManagerWidget::enableButtons(bool enable){
    if(enable){
        slotAdaptButtonsStateToSelection();
    }else{
        buttonConfidence->setEnabled(enable);
        buttonDrop->setEnabled(enable);
        buttonOnline->setEnabled(enable);
    }
}
