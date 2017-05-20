#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include "jsonparser.h"
#include "dbmanager.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_lineEdit_returnPressed()
{
    qDebug()<<"enter was pressed";
    QString id = ui->lineEdit->text();
    json.initParser(id);
    ui->lineEdit_2->setText(json.getName());
    ui->comboBox->addItems(json.getPlatformList());
    imageDownloader(json.getImage(),json.getName());
    QPixmap pixmap = imageViewer(json.getName());
    ui->label_3->setPixmap(pixmap);


}

void Dialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    json.setPlatform(ui->comboBox->currentText());

}

void Dialog::on_pushButton_3_clicked()
{
    dbmanager db;
    db.autoAddEntry(json.getMap(),json.getGenre());
    qDebug()<<"ADDED!!!!!!!!!!!!!";
}
