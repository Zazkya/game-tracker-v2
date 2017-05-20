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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    dbmanager db;
//    db.createTable();
//    db.createGenreTable();
//    db.createLinkerTable();
//    jsonparser j("5");
//    bool success = db.autoAddEntry(j.getMap(),j.getGenre(), j.getPlatformList());
//    qDebug()<<success;
//    QSqlQueryModel *model = new QSqlQueryModel();
//    model->setQuery(db.queryAll());
//    ui->tableView->setModel(model);
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QItemSelectionModel *select = ui->tableView->selectionModel();
//   // QObject::connect(select, SIGNAL(currentRowChanged(QModelIndex))), this, SLOT(on_tableView_clicked(QModelIndex));
//    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget_2->hide();
    ui->verticalLayout_2->removeWidget(ui->stackedWidget_2);
    ui->verticalLayout_2->update();
    ui->stackedWidget->adjustSize();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget_2->show();
    ui->verticalLayout_2->addWidget(ui->stackedWidget_2);
    ui->verticalLayout_2->update();
}

void MainWindow::on_pushButton_2_clicked()
{



}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{



}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    const QAbstractItemModel *model = index.model();

    QString name = model->data(model->index(index.row(), 0), 0).toString();
    QString newName = prettyString(name);

    QString myPath = QCoreApplication::applicationDirPath();
    QString folderPath = myPath +"/"+ newName;
    QString path =folderPath + "/" + newName + ".jpg";

    QPixmap pixmap(path);
    QPixmap newp = pixmap.scaled(250,250,Qt::KeepAspectRatio);
    ui->label->setPixmap(newp);
}

void MainWindow::on_pushButton_8_clicked()
{
    Dialog dl;
    dl.setModal(true);
    dl.exec();
}
