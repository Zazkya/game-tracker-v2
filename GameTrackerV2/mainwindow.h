#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include "QSqlQueryModel"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_viewButton_clicked();
    void on_addButton_clicked();
    void on_allButton_clicked();
    void on_finishedButton_clicked();
    void on_unfinishedButton_clicked();
    void on_retiredButton_clicked();
    void on_editButton_clicked();
    void updateQuery();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_platformFilter_currentIndexChanged(const QString &arg1);
    void tableSetup();
    QSqlQueryModel* tableQuery(QString status);
    void panelSetup();
    void buttonColor();


    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString currentName;
    int currentQuery;
    QList<QString> platformList;
    QString currentFilter;
    QString searchText;
};
#endif // MAINWINDOW_H
