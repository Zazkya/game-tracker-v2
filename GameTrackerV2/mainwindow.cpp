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
#include "dialog.h"
#include <QDir>
#include <QFile>
#include "global.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbmanager db;
    db.createTable();
    db.createGenreTable();
    db.createLinkerTable();

//    QItemSelectionModel *select = ui->tableView->selectionModel();
//   // QObject::connect(select, SIGNAL(currentRowChanged(QModelIndex))), this, SLOT(on_tableView_clicked(QModelIndex));
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
    QString newName = prettyString(name);

    QString myPath = QCoreApplication::applicationDirPath();
    QString folderPath = myPath + "/" + "Images" +"/"+ newName;
    QString path =folderPath + "/" + newName + ".jpg";

    if(!QFile(path).exists()){
       path = folderPath + "/" + newName + ".png";
    }

    QPixmap pixmap(path);
    QPixmap newp = pixmap.scaled(250,250,Qt::KeepAspectRatio);
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
    QItemSelectionModel *select = ui->tableView->selectionModel();
    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
}
