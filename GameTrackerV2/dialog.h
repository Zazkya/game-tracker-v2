#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "jsonparser.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_lineEdit_returnPressed();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;
    jsonparser json;
};

#endif // DIALOG_H
