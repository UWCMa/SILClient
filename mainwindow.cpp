#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QMetaEnum>
#include <QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tracesarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow (parent)
    , ui          (new Ui::MainWindow)
    , mMovieLoad  (QString( ":/icons/loader.gif"))
    , mProcess    (new QProcess(parent))
    , mBinaryPath ("")
    , mRunPeriod  ("40")
    , isRuning    (false)
{
    ui->setupUi(this);
    mMovieLoad.setScaledSize(this->ui->LoadAnimation->size());
    this->ui->LoadAnimation->setMovie(&mMovieLoad);
    setLabelIcon(this->ui->labelFrog, ":/icons/frog.png");

    connect(mProcess, &QProcess::readyReadStandardOutput, [this](){
        this->ui->tracesArea->outputMessage(QtInfoMsg, QMessageLogContext()
                                       , mProcess->readAllStandardOutput());
    });
    connect(mProcess, &QProcess::readyReadStandardError, [this](){
        this->ui->tracesArea->outputMessage(QtCriticalMsg, QMessageLogContext()
                                            , mProcess->readAllStandardError());
        if(QMovie::MovieState::Running == mMovieLoad.state())mMovieLoad.setPaused(true);
        setEnabledBtn(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    static_cast<void>(event);
    if(isProcessRunning())
       mProcess->kill();
}

void MainWindow::on_btnRun_clicked()
{
    if(false == isProcessRunning())
    {
        if(mBinaryPath.isEmpty())
        {
            QString msg("Path to the binary file is missed");
            if(warningMessage(msg))
                return;
        }

        QStringList arguments;
        arguments << mRunPeriod;
        mProcess->start(mBinaryPath, arguments);
        if(!isProcessRunning())
        {
            QString msg("Unable to start this file!");
            this->ui->tracesArea->clear();
            if(!this->ui->LoadAnimation->isHidden())
                this->ui->LoadAnimation->hide();
            mProcess->terminate();
            if(warningMessage(msg))
                return;
        }
        else
        {
            mMovieLoad.start();
            setEnabledBtn(false);
        }
    }
    this->ui->LoadAnimation->show();
}

void MainWindow::on_btnShutdown_clicked()
{
    if(true == isProcessRunning())
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<eProcessCmd>();
        QString text = metaEnum.valueToKey(SIL_SHUTDOWN);
        writeToStdin(text);
    }
    mMovieLoad.setPaused(true);
    setEnabledBtn(true);
}

void MainWindow::on_actionPath_to_a_binary_triggered()
{
    mBinaryPath = QFileDialog::getOpenFileName(this,
                                         "Select binary file",
                                         "/home",
                                         "All files (*.*)");
    if(isProcessRunning())
    {
        mProcess->kill();
    }
    if(!mBinaryPath.isEmpty())
    {
        mMovieLoad.start();
        mMovieLoad.stop();
        if(this->ui->LoadAnimation->isHidden())
            this->ui->LoadAnimation->show();
    }
    else
        this->ui->LoadAnimation->hide();
}

bool MainWindow::warningMessage(const QString& msg)
{
    return QMessageBox::Ok ==
           QMessageBox::warning(this, "WARNING", msg);
}

void MainWindow::on_actionSet_Run_period_triggered()
{
    bool ok = false;
    QString text = QInputDialog::getText(0, "",
                                         "Run Period(ms):", QLineEdit::Normal,
                                          mRunPeriod, &ok);
    if(!ok || text.isEmpty())
        return;
    if(20 > text.toInt() || 200 < text.toInt())
    {
        QString msg("The running period should be in the range(20 - 200)");
        warningMessage(msg);
    }
    else
    {
        this->ui->labelPeriodValue->setText(text + " ms");
        mRunPeriod = text;
    }
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
    }
}

bool MainWindow::isProcessRunning() const
{
    return QProcess::NotRunning == mProcess->state()
            ? false : true;
}

void MainWindow::setLabelIcon(QLabel* label, const QString& path)
{
    QPixmap picture(path);
    label->setPixmap(picture);
    label->setScaledContents(true);
    label->show();
}

void MainWindow::setEnabledBtn(bool enabled)
{
    this->ui->btnRun->                setEnabled(enabled);
    this->ui->btnClear->              setEnabled(enabled);
    this->ui->actionPath_to_a_binary->setEnabled(enabled);
    this->ui->actionSet_Run_period->  setEnabled(enabled);
    this->ui->actionSave_to_File->    setEnabled(enabled);
}

void MainWindow::on_actionSave_to_File_triggered()
{
    this->ui->tracesArea->saveLogsToFileSytem();
}

void MainWindow::on_btnClear_clicked()
{
    this->ui->tracesArea->clear();
}


void MainWindow::on_btnInfo_clicked()
{
    QMessageBox::StandardButton reply;
    QString info("This app was developed to have the ability to test Data Logger."
                 "If you are brave developer and open to this chalange you can "
                 "choose a binary, set up a value of cycle and enjoy it");
    reply = QMessageBox::information(this, "Info", info);
}
