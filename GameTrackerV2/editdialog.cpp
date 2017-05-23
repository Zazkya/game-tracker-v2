#include "editdialog.h"
#include "ui_editdialog.h"

editdialog::editdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editdialog)
{
    ui->setupUi(this);
}

editdialog::~editdialog()
{
    delete ui;
}

bool editdialog::setName(QString gameName){
    name = gameName;
    return true;

}

bool editdialog::dbQuery(){
    editDialogMap = editDialogdb.editQuery(name);
}

bool editdialog::setQuery(){
    ui->nameEdit->setText(editDialogMap["name"]);
    ui->nameEdit->setReadOnly(true);
    ui->platformEdit->setText(editDialogMap["platform"]);
    ui->platformEdit->setReadOnly(true);
    ui->statusBox->addItem(editDialogMap["status"]);
    ui->commentEdit->setText(editDialogMap["notes"]);

    if(editDialogMap["status"] == "Unfinished"){
        ui->statusBox->addItem("Finished");
        ui->statusBox->addItem("Retired");
    }else if (editDialogMap["status"] == "Finished") {
        ui->statusBox->addItem("Unfinished");
        ui->statusBox->addItem("Retired");
    }else {
        ui->statusBox->addItem("Unfinished");
        ui->statusBox->addItem("Finished");
    }


}

void editdialog::on_statusBox_currentIndexChanged(const QString &arg1){
    editDialogMap["status"] = ui->statusBox->currentText();
}

void editdialog::on_buttonBox_accepted(){
    editDialogdb.updateNote(editDialogMap["notes"], name);
    editDialogdb.updateStatus(editDialogMap["status"], name);

}

void editdialog::on_commentEdit_textChanged(){
    editDialogMap["notes"] = ui->commentEdit->toPlainText();
}

void editdialog::on_buttonBox_rejected(){
    this->close();
}
