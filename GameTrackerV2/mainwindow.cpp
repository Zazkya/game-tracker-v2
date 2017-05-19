#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "jsonparser.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);





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
