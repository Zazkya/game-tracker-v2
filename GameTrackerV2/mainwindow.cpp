#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "jsonparser.h"
#include "dbmanager.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QString>
#include <QObject>
#include <QSortFilterProxyModel>
#include "dialog.h"
#include <QDir>
#include <QFile>
#include "global.h"
#include "editdialog.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbmanager db;
    db.createTable();
    db.createGenreTable();
    db.createLinkerTable();
    on_allButton_clicked();
//    QItemSelectionModel *select = ui->tableView->selectionModel();
//    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
}

MainWindow::~MainWindow(){
    delete ui;
}

/**
 * @brief MainWindow::on_tableView_clicked
 * @param index
 * loads image to sidebar when a row is clicked
 */

void MainWindow::on_tableView_clicked(const QModelIndex &index){
    const QAbstractItemModel *model = index.model();

    QString name = model->data(model->index(index.row(), 0), 0).toString();
    currentName = name;
    QString newName = prettyString(name);

    QString myPath = QCoreApplication::applicationDirPath();
    QString folderPath = myPath + "/" + "Images" +"/"+ newName;
    QString path =folderPath + "/" + newName + ".jpg";

    if(!QFile(path).exists()){
       path = folderPath + "/" + newName + ".png";
    }

    QPixmap pixmap(path);
    QPixmap newp = pixmap.scaled(200,250,Qt::IgnoreAspectRatio);
    ui->label->setPixmap(newp);
}

/**
 * @brief MainWindow::on_viewButton_clicked
 * hides and shows bottom panel
 */
void MainWindow::on_viewButton_clicked()
{
    if(ui->scrollArea->isHidden()){
        ui->scrollArea->show();
        ui->verticalLayout_2->addWidget(ui->scrollArea);
        ui->verticalLayout_2->update();
    }else{
        ui->scrollArea->hide();
        ui->verticalLayout_2->removeWidget(ui->scrollArea);
        ui->verticalLayout_2->update();
        ui->stackedWidget->adjustSize();
    }
}

/**
 * @brief MainWindow::on_addButton_clicked
 * creates add window dialog when add button clicked
 */
void MainWindow::on_addButton_clicked()
{
    Dialog dl;
    dl.setModal(true);
    dl.exec();
}

/**
 * @brief MainWindow::on_allButton_clicked
 * shows all entries in database in table
 */
void MainWindow::on_allButton_clicked()
{
    dbmanager db;
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(db.queryAll());
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setDynamicSortFilter(true);
    sort_filter->setSourceModel(model);
    ui->tableView->setModel(sort_filter);
    ui->tableView->setSortingEnabled(true);
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));

}

void MainWindow::on_finishedButton_clicked()
{
    dbmanager db;
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(db.queryStatus("Finished"));
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setDynamicSortFilter(true);
    sort_filter->setSourceModel(model);
    ui->tableView->setModel(sort_filter);
    ui->tableView->setSortingEnabled(true);
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
}

void MainWindow::on_unfinishedButton_clicked()
{
    dbmanager db;
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(db.queryStatus("Unfinished"));
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setDynamicSortFilter(true);
    sort_filter->setSourceModel(model);
    ui->tableView->setModel(sort_filter);
    ui->tableView->setSortingEnabled(true);
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
}

void MainWindow::on_retiredButton_clicked()
{
    dbmanager db;
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(db.queryStatus("Retired"));
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setDynamicSortFilter(true);
    sort_filter->setSourceModel(model);
    ui->tableView->setModel(sort_filter);
    ui->tableView->setSortingEnabled(true);
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));

}

void MainWindow::on_editButton_clicked(){
    editdialog dl;
    dl.setModal(true);
    dl.setName(currentName);
    dl.dbQuery();
    dl.setQuery();



    dl.exec();
}
