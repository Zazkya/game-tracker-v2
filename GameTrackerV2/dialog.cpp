#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include "jsonparser.h"
#include "dbmanager.h"
#include "global.h"



/**
 * @brief Dialog::Dialog
 * @param parent
 * creates window for searching api and adding to db auto and manually
 */
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

/**
 * @brief Dialog::on_lineEdit_returnPressed
 * Gets id from line edit and does api request to get data
 * Puts platforms in combobox
 * downloads related picture and shows it
 */
void Dialog::on_lineEdit_returnPressed(){
    QString id = ui->lineEdit->text();
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    ui->progressBar->setToolTip("Downloading Information");
    json.initParser(id);
    ui->lineEdit_2->setText(json.getName()); 
    ui->comboBox->addItems(json.getPlatformList());
    ui->progressBar->setValue(50);
    ui->progressBar->setToolTip("Downloading Image");
    imageDownloader(json.getImage(),json.getName());
    QPixmap pixmap = imageViewer(json.getName());
    ui->label_3->setPixmap(pixmap);
    ui->progressBar->setValue(100);
    ui->progressBar->setToolTip("Done");
}

/**
 * @brief Dialog::on_comboBox_currentIndexChanged
 * @param arg1
 * sets platform in map to current selection in combobox
 */
void Dialog::on_comboBox_currentIndexChanged(const QString &arg1){
    json.setPlatform(ui->comboBox->currentText());
}

/**
 * @brief Dialog::on_pushButton_3_clicked
 * if add button is clicked then add to db
 */
void Dialog::on_pushButton_3_clicked(){
    dbmanager db;
    db.autoAddEntry(json.getMap(),json.getGenre());
    this->close();
}
