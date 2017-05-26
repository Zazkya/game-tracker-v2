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
    setWindowIcon(QIcon(":/new/icons/Images/Icons/057-joystick.png"));

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
      qDebug()<<"Unable to set stylesheet, file not found\n";
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    dbmanager db;
    db.createTable();
    db.createGenreTable();
    db.createLinkerTable();

    platformList = db.getUniquePlatforms();
    ui->platformFilter->addItems(platformList);
    currentQuery = 0;
    this->on_viewButton_clicked();
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
    this->panelSetup();
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
    this->updateQuery();
}

/**
 * @brief MainWindow::on_allButton_clicked
 * shows all entries in database in table
 */
void MainWindow::on_allButton_clicked()
{
    currentQuery = 0;
     this->tableSetup();
//    this->buttonColor();
//    dbmanager db;
//    QSqlQueryModel *model = new QSqlQueryModel();
//    model->setQuery(db.queryAll());
//    ui->tableView->setModel(model);
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
//    sort_filter->setDynamicSortFilter(true);
//    sort_filter->setSourceModel(model);
//    ui->tableView->setModel(sort_filter);
//    ui->tableView->setSortingEnabled(true);
//    QItemSelectionModel *select = ui->tableView->selectionModel();
//    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));

}

void MainWindow::on_finishedButton_clicked()
{
    currentQuery = 1;
    this->tableSetup();
//    this->buttonColor();
//    dbmanager db;
//    QSqlQueryModel *model = new QSqlQueryModel();
//    model->setQuery(db.queryStatus("Finished"));
//    ui->tableView->setModel(model);
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
//    sort_filter->setDynamicSortFilter(true);
//    sort_filter->setSourceModel(model);
//    ui->tableView->setModel(sort_filter);
//    ui->tableView->setSortingEnabled(true);
//    QItemSelectionModel *select = ui->tableView->selectionModel();
//    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
}

void MainWindow::on_unfinishedButton_clicked()
{
    currentQuery = 2;
//    this->buttonColor();
     this->tableSetup();
//    dbmanager db;
//    QSqlQueryModel *model = new QSqlQueryModel();
//    model->setQuery(db.queryStatus("Unfinished"));
//    ui->tableView->setModel(model);
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
//    sort_filter->setDynamicSortFilter(true);
//    sort_filter->setSourceModel(model);
//    ui->tableView->setModel(sort_filter);
//    ui->tableView->setSortingEnabled(true);
//    QItemSelectionModel *select = ui->tableView->selectionModel();
//    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
}

void MainWindow::on_retiredButton_clicked()
{
    currentQuery = 3;
//    this->buttonColor();
     this->tableSetup();
//    dbmanager db;
//    QSqlQueryModel *model = new QSqlQueryModel();
//    model->setQuery(db.queryStatus("Retired"));
//    ui->tableView->setModel(model);
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
//    sort_filter->setDynamicSortFilter(true);
//    sort_filter->setSourceModel(model);
//    ui->tableView->setModel(sort_filter);
//    ui->tableView->setSortingEnabled(true);
//    QItemSelectionModel *select = ui->tableView->selectionModel();
//    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));

}

void MainWindow::on_editButton_clicked(){
    editdialog dl;
    dl.setModal(true);
    dl.setName(currentName);
    dl.dbQuery();
    dl.setQuery();
    dl.exec();
    this->updateQuery();

}

void MainWindow::updateQuery(){

    switch (currentQuery) {
        case 0:
            on_allButton_clicked();
            break;
        case 1:
            on_finishedButton_clicked();
            break;
        case 2:
            on_unfinishedButton_clicked();
            break;
        case 3:
            on_retiredButton_clicked();
            break;
    }
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index){
    on_editButton_clicked();
}

void MainWindow::on_platformFilter_currentIndexChanged(const QString &arg1){
    currentFilter = ui->platformFilter->currentText();
    this->updateQuery();
}

void MainWindow::tableSetup(){
    QSqlQueryModel *model = new QSqlQueryModel();

    switch (currentQuery) {
        case 0:
            model = this->tableQuery("All");
            break;
        case 1:
            model = this->tableQuery("Finished");
            break;
        case 2:
            model = this->tableQuery("Unfinished");
            break;
        case 3:
            model = this->tableQuery("Retired");
            break;
      }

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setDynamicSortFilter(true);
    sort_filter->setSourceModel(model);
    sort_filter->setFilterWildcard(QRegExp(searchText,Qt::CaseInsensitive,QRegExp::Wildcard).pattern());

    sort_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->tableView->setModel(sort_filter);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->horizontalHeader()->setDragEnabled(true);
    ui->tableView->setColumnWidth(0,195);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);




    QItemSelectionModel *select = ui->tableView->selectionModel();
    QObject::connect(select,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
}

QSqlQueryModel* MainWindow::tableQuery(QString status){
    dbmanager db;
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(db.queryPlatform(currentFilter, status));
    model->setHeaderData(0, Qt::Horizontal, "Title");
    model->setHeaderData(1, Qt::Horizontal, "Platform");
    model->setHeaderData(2, Qt::Horizontal, "Status");
    model->setHeaderData(3, Qt::Horizontal, "Date Added");
    model->setHeaderData(4, Qt::Horizontal, "Date Modified");
    model->setHeaderData(5, Qt::Horizontal, "Comment");
    return model;

}

void MainWindow::panelSetup(){
    dbmanager db;

    QList<QString> list = db.getGenre(currentName);
    QString genre;
    ui->titleLabel->setText(currentName);
    ui->descriptionLabel->setText(db.getSynopsis(currentName));

    genre = list[0];
    for(int i = 1; i < list.count(); i++){
        genre.append(", " + list[i]);
    }

    ui->genreLabel->setText(genre);
    ui->platformLabel->setText(db.getPlatform(currentName));
    ui->devLabel->setText(db.getDeveloper(currentName));
    ui->publisherLabel->setText(db.getPublisher(currentName));
    ui->seriesLabel->setText(db.getSeries(currentName));
    ui->deckLabel->setText(db.getDeck(currentName));
}

void MainWindow::buttonColor(){
    switch (currentQuery) {
    case 0:
        ui->allButton->setStyleSheet("background-color: #3daee9");
        ui->finishedButton->setStyleSheet("background-color: #31363b");
        ui->unfinishedButton->setStyleSheet("background-color: #31363b");
        ui->retiredButton->setStyleSheet("background-color: #31363b");
        break;
    case 1:
        ui->finishedButton->setStyleSheet("background-color: #3daee9");
        ui->allButton->setStyleSheet("background-color: #31363b");
        ui->unfinishedButton->setStyleSheet("background-color: #31363b");
        ui->retiredButton->setStyleSheet("background-color: #31363b");
        break;
    case 2:
        ui->unfinishedButton->setStyleSheet("background-color: #3daee9");
        ui->finishedButton->setStyleSheet("background-color: #31363b");
        ui->allButton->setStyleSheet("background-color: #31363b");
        ui->retiredButton->setStyleSheet("background-color: #31363b");
        break;
    case 3:
        ui->retiredButton->setStyleSheet("background-color: #3daee9");
        ui->finishedButton->setStyleSheet("background-color: #31363b");
        ui->unfinishedButton->setStyleSheet("background-color: #31363b");
        ui->allButton->setStyleSheet("background-color: #31363b");
        break;
    }

}



void MainWindow::on_lineEdit_textChanged(const QString &arg1){
    searchText = arg1;
    this->updateQuery();

}










