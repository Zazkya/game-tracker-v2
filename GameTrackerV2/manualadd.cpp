#include "manualadd.h"
#include "ui_manualadd.h"
#include "dbmanager.h"
#include "jsonparser.h"
#include "QDebug"

manualAdd::manualAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manualAdd)
{
    ui->setupUi(this);
}

manualAdd::~manualAdd()
{
    delete ui;
}

void manualAdd::on_buttonBox_accepted(){
    dbmanager db;
    QMap<QString, QString> map;
    QList<QString> genreList;

    map["name"] = ui->nameEdit->text();
    map["platform"] = ui->platformEdit->text();
    map["status"] = "Unfinished";
    map["description"] = ui->descriptionEdit->toPlainText();
    map["deck"] = ui->deckEdit->text();
    map["developer"] = ui->devEdit->text();
    map["publisher"] = ui->pubEdit->text();
    map["series"] = ui->franchiseEdit->text();
    map["image"] = ui->imageEdit->text();
    genreList.append(ui->genreEdit->text());

    db.autoAddEntry(map,genreList);

    imageDownloader(map["image"], map["name"]);


}
