#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
#include "tracesarea.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

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
    , mMovieLoad    (QString( ":/icons/ajax-loader.gif"))
    , mMovieInitS   (QString( ":/icons/success.png"    ))
    , mMovieShutdown(QString( ":/icons/off_1.png"      ))
    , mProcess(new QProcess(parent))
    , mBinaryPath("")
{
    ui->setupUi(this);

    //QLabel
    mMovieLoad.setScaledSize    (this->ui->LoadAnimation->size());
    mMovieInitS.setScaledSize   (this->ui->LoadAnimation->size());
    mMovieShutdown.setScaledSize(this->ui->LoadAnimation->size());

    this->ui->LoadAnimation->setMovie(&mMovieLoad);
    this->ui->InitSuccess->setMovie  (&mMovieInitS);
    this->ui->Shutdown->setMovie     (&mMovieShutdown);

    connect(mProcess, &QProcess::readyReadStandardOutput, [this](){
        QString output =mProcess->readAllStandardOutput();
        qDebug() << "output: "<< output;
    });

    connect(mProcess, &QProcess::readyReadStandardError, [this](){
        QString err = mProcess->readAllStandardError();
        qDebug() << "error: "<<err;
    });

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

    QString program = "C:/Users/skobz/source/repos/forqt/Debug/forqt.exe";
    QStringList arguments;
    //arguments << "-style" << "motif";
    mProcess->start(program);

}

void MainWindow::on_pushButton_clicked()
{

    mMovieInitS.start();
}

void MainWindow::on_pushButton_4_clicked()
{
    mMovieShutdown.start();
    mProcess->write("Shutdown!!!!");
    mProcess->waitForBytesWritten();
    mProcess->closeWriteChannel();
}
//////////////////////////////////////////
void MainWindow::on_btnRun_clicked()
{
    if(mBinaryPath.isEmpty())
    {
        QString msg("The binary path is missed");
        if(warrningMessage(msg))
            return;
    }

    QStringList arguments; // NOT used
    mProcess->start(mBinaryPath);
    if(QProcess::NotRunning == mProcess->state())
    {
        QString msg("The binary was not executed!");
        if(warrningMessage(msg))
            return;
    }
    else
    {
        mMovieLoad.start();
    }
}

void MainWindow::on_btnShutdown_clicked()
{

}

void MainWindow::on_actionPath_to_a_binary_triggered()
{
   /* mBinaryPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    qDebug() << "Binary directory: "<< mBinaryPath;*/
    mBinaryPath = QFileDialog::getOpenFileName(this,
                                         "Select binary file",
                                         "/home",
                                         "All files (*.*)");
}

bool MainWindow::warrningMessage(const QString& msg)
{
    bool clicked = false;
    qDebug() << "WARNING: " << msg;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "WARNING", msg);
    if (reply == QMessageBox::Ok) {
        clicked = true;
    }
    return clicked;
}
