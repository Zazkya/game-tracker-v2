#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include "dbmanager.h"
#include <QMap>

namespace Ui {
class editdialog;
}

class editdialog : public QDialog
{
    Q_OBJECT

public:
    explicit editdialog(QWidget *parent = 0);
    ~editdialog();
    bool setName(QString gameName);
    bool dbQuery();
    bool setQuery();

private slots:
    void on_statusBox_currentIndexChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_commentEdit_textChanged();

    void on_buttonBox_rejected();

private:
    Ui::editdialog *ui;
    dbmanager editDialogdb;
    QString name;
    QMap<QString, QString> editDialogMap;
};

#endif // EDITDIALOG_H
