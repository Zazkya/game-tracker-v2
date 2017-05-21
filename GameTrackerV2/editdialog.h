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

private:
    Ui::editdialog *ui;
    dbmanager editDialogdb;
    QString name;
    QMap<QString, QString> editDialogMap;
};

#endif // EDITDIALOG_H
