#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
#include "tracesarea.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

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
    , mMovieShutdown(QString( ":/icons/inactive.png" ))
    , mProcess(new QProcess(parent))
    , mBinaryPath("")
    , isRuning(false)
    , mRunPeriod(40)
{
    ui->setupUi(this);

    //QLabel
    mMovieLoad.setScaledSize    (this->ui->LoadAnimation->size());
    mMovieInitS.setScaledSize   (this->ui->LoadAnimation->size());
    mMovieShutdown.setScaledSize(this->ui->LoadAnimation->size());

    this->ui->LoadAnimation->setMovie(&mMovieLoad);
    this->ui->Shutdown->setMovie     (&mMovieShutdown);

    mMovieLoad.start(); // REDID just to see picture
    mMovieLoad.stop();
    mMovieShutdown.start();
    mMovieShutdown.stop();

    connect(mProcess, &QProcess::readyReadStandardOutput, [this](){
        QString output =mProcess->readAllStandardOutput();
        qDebug() << "output: "<< output;
    });

    connect(mProcess, &QProcess::readyReadStandardError, [this](){
        QString err = mProcess->readAllStandardError();
        qDebug() << "error: "<<err;
    });

    QPixmap picClear(":/icons/clear.png");
    this->ui->labelClear->setPixmap(picClear);
    this->ui->labelClear->setScaledContents(true);
    this->ui->labelClear->show();

    QPixmap picFrog(":/icons/frog.png");
    this->ui->labelFrog->setPixmap(picFrog);
    this->ui->labelFrog->setScaledContents(true);
    this->ui->labelFrog->show();
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

//////////////////////////////////////////
void MainWindow::on_btnRun_clicked()
{
    if(false == isRuning)
    {
        if(mBinaryPath.isEmpty())
        {
            QString msg("The binary path is missed");
            if(warningMessage(msg))
                return;
        }

        QStringList arguments; // NOT used
        mProcess->start(mBinaryPath);
        if(QProcess::NotRunning == mProcess->state())
        {
            QString msg("The binary was not executed!");
            mProcess->terminate();
            if(warningMessage(msg))
                return;
        }
        else
        {
            this->ui->LoadAnimation->setMovie(&mMovieLoad);
            mMovieLoad.start();
            mMovieShutdown.setFileName(QString( ":/icons/active.png" ));
            mMovieShutdown.start();
            mMovieShutdown.stop();
            isRuning = true;
        }
    }
}

void MainWindow::on_btnShutdown_clicked()
{
    if(isRuning)
    {
        mMovieLoad.stop();

        mMovieShutdown.setFileName(QString( ":/icons/inactive.png" ));
        mMovieShutdown.start();
        mMovieShutdown.stop();
        mProcess->write("shutdown");//TODO normal typeof message
        mProcess->waitForBytesWritten();
        mProcess->closeWriteChannel();
        isRuning = false;
    }
}

void MainWindow::on_actionPath_to_a_binary_triggered()
{
    mBinaryPath = QFileDialog::getOpenFileName(this,
                                         "Select binary file",
                                         "/home",
                                         "All files (*.*)");
}

bool MainWindow::warningMessage(const QString& msg)
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

void MainWindow::on_actionSet_Run_period_triggered()
{
    //bool ok = true;
    QString text = QInputDialog::getText(0, "",
                                         "Run Period(ms):", QLineEdit::Normal,
                                         "", nullptr);
    this->ui->labelPeriodValue->setText(text); // add to check of number
    //add check!!
    QByteArray array = text.toLocal8Bit();
    char* buffer = array.data();
    mProcess->write(buffer);//TODO normal typeof message
    mProcess->waitForBytesWritten();
    mProcess->closeWriteChannel();
}
