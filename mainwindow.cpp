#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
#include "tracesarea.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QMetaEnum>

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
    , mRunPeriod("40")
    , isRuning(false)
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
        QMessageLogContext context;
        this->ui->tracesArea->outputMessage(QtDebugMsg, context, output);
        //qDebug() << "output: "<< output;
    });

    connect(mProcess, &QProcess::readyReadStandardError, [this](){
        QString err = mProcess->readAllStandardError();
        QMessageLogContext context;
        this->ui->tracesArea->outputMessage(QtCriticalMsg, context, err);
       // qDebug() << "error: "<<err;
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
    //mProcess->terminate();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Main window is closed";
    if(isProcessRunning())
    {
        mProcess->terminate();
        mProcess->kill();
    }
}

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

        QStringList arguments;
        arguments << mRunPeriod;
        mProcess->start(mBinaryPath, arguments);
        if(!isProcessRunning())
        {
            QString msg("The binary was not executed!");
            mProcess->terminate();
            if(warningMessage(msg))
                return;
        }
        else
        {
            qDebug() << "Running!!!";
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
    if(true == isRuning && isProcessRunning())
    {
        qDebug() << "Shutdown\n";

        QMetaEnum metaEnum = QMetaEnum::fromType<eProcessCmd>();
        QString text = metaEnum.valueToKey(SIL_SHUTDOWN);
        writeToStdin(text);
    }
    mMovieShutdown.setFileName(QString( ":/icons/inactive.png" ));
    mMovieShutdown.start();
    mMovieShutdown.stop();
    mMovieLoad.stop();
    isRuning = false;
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
    mRunPeriod = text;

}

void MainWindow::writeToStdin(const QString& text)
{
    if(isProcessRunning())
    {
        qDebug() << "Sending Signal to process: " << text;
        QByteArray array = text.toLocal8Bit();
        char* buffer = array.data();
        qint64 bytes = mProcess->write(buffer);
        mProcess->waitForBytesWritten();
        mProcess->closeWriteChannel();
        qDebug() << "bytes = " << bytes;
        qDebug() << "text = " << text;
        qDebug() << "state: " << mProcess->state();


    }
}

bool MainWindow::isProcessRunning() const
{
    return QProcess::NotRunning == mProcess->state()
           ? false : true;
}

void MainWindow::on_actionSave_to_File_triggered()
{
    this->ui->tracesArea->save();
}

void MainWindow::on_btnClear_clicked()
{
    this->ui->tracesArea->clear();
}
