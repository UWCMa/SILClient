#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>

#include <QtGlobal>
#include "logbrowser.h"
#include <qapplication.h>

#include "tracesarea.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
  {
      QByteArray localMsg = msg.toLocal8Bit();
      const char *file = context.file ? context.file : "";
      const char *function = context.function ? context.function : "";
      switch (type) {
      case QtDebugMsg:
          fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
          break;
      case QtInfoMsg:
          fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
          break;
      case QtWarningMsg:
          fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
          break;
      case QtCriticalMsg:
          fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
          break;
      case QtFatalMsg:
          fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
          break;

          ////////////////////

      }
  }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mMovieLoad(QString(":/icons/ajax-loader.gif"))
    , mMovieInitS(QString(":/icons/success.png"))
    //, mMovieInitS(QString(":/icons/success.png"))
    //, mInitFailed(QString(":/icons/failed.png"))
    , mMovieShutdown(QString(":/icons/off_1.png"))
{
    ui->setupUi(this);
    this->ui->btnStart->setIcon(QIcon("start.png"));
    this->ui->btnStart->setIconSize(QSize(65, 65));
    QString buttonStyle = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
    this->ui->btnStart->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");

    //QLabel
    mMovieLoad.setScaledSize(this->ui->LoadAnimation->size());
    mMovieInitS.setScaledSize(this->ui->LoadAnimation->size());
    //mInitFailed.setScaledSize(this->ui->LoadAnimation->size());
    mMovieShutdown.setScaledSize(this->ui->LoadAnimation->size());
    this->ui->LoadAnimation->setMovie(&mMovieLoad);
    this->ui->InitSuccess->setMovie(&mMovieInitS);
    this->ui->Shutdown->setMovie(&mMovieShutdown);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Run_clicked()
{
    mMovieLoad.start();
    QMessageLogContext context(__FILE__, __LINE__, __FUNCTION__, "QtWarningMsg");
    this->ui->tracesArea->outputMessage(QtCriticalMsg, context, "Hello");

}

void MainWindow::on_pushButton_clicked()
{
    mMovieInitS.start();
}

void MainWindow::on_pushButton_4_clicked()
{
    mMovieShutdown.start();
}
