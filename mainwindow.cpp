#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->btnStart->setIcon(QIcon("start.png"));
    this->ui->btnStart->setIconSize(QSize(65, 65));
    QString buttonStyle = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
   this->ui->btnStart->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");
    //this->ui->btnStart->setStyleSheet(buttonStyle);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

}
