#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mMovieLoad(QString("C:/Users/skobz/Development/QtPractice/SILClient/icons/ajax-loader.gif"))
    , mMovieInitS(QString("C:/Users/skobz/Development/QtPractice/SILClient/icons/success.png"))
    , mInitFailed(QString("C:/Users/skobz/Development/QtPractice/SILClient/icons/failed.png"))
    , mMovieShutdown(QString("C:/Users/skobz/Development/QtPractice/SILClient/icons/off_1.png"))
{
    ui->setupUi(this);
    this->ui->btnStart->setIcon(QIcon("start.png"));
    this->ui->btnStart->setIconSize(QSize(65, 65));
    QString buttonStyle = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
    this->ui->btnStart->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");
    //this->ui->btnStart->setStyleSheet(buttonStyle);

    //QLabel
    mMovieLoad.setScaledSize(this->ui->LoadAnimation->size());
    mMovieInitS.setScaledSize(this->ui->LoadAnimation->size());
    mInitFailed.setScaledSize(this->ui->LoadAnimation->size());
    mMovieShutdown.setScaledSize(this->ui->LoadAnimation->size());
    this->ui->LoadAnimation->setMovie(&mMovieLoad);
    this->ui->InitSuccess->setMovie(&mMovieInitS);
    this->ui->Shutdown->setMovie(&mMovieShutdown);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_2_clicked()
//{
//    QLabel *lbl = new QLabel;
//    QMovie *movie = new QMovie(QString("C:/Users/skobz/Development/QtPractice/SILClient/icons/ajax-loader.gif"));
//    lbl->setMovie(movie);
//    lbl->show();
//    movie->start();
//}

void MainWindow::on_Run_clicked()
{
    mMovieLoad.start();
}

void MainWindow::on_pushButton_clicked()
{
    mMovieInitS.start();
}

void MainWindow::on_pushButton_4_clicked()
{
    mMovieShutdown.start();
}
