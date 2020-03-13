#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QMetaEnum>
#include <QLabel>
#include <QMovie>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tracesarea.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow (parent)
    , ui          (new Ui::MainWindow)
    , mMovieLoad  (new QMovie(":/icons/loader.gif"))
    , mProcess    (new QProcess(parent))
    , mBinaryPath ("")
    , mRunPeriod  ("40")
    , isRuning    (false)
{
    ui->setupUi(this);
    mMovieLoad->setScaledSize(this->ui->LoadAnimation->size());
    this->ui->LoadAnimation->setMovie(mMovieLoad);
    setLabelIcon(this->ui->labelFrog, ":/icons/frog.png");

    connect(mProcess, &QProcess::readyReadStandardOutput, [this](){
        this->ui->tracesArea->outputMessage(QtInfoMsg, QMessageLogContext(),
                                            mProcess->readAllStandardOutput());
    });
    connect(mProcess, &QProcess::readyReadStandardError, [this](){
        this->ui->tracesArea->outputMessage(QtCriticalMsg, QMessageLogContext(),
                                            mProcess->readAllStandardError());
        if(QMovie::MovieState::Running == mMovieLoad->state())
        {
            mMovieLoad->setPaused(true);
            if(!this->ui->LoadAnimation->isHidden())
                this->ui->LoadAnimation->hide();
        }
        setEnabledBtn(true);
    });
}

MainWindow::~MainWindow()
{
    if(nullptr != mMovieLoad) delete mMovieLoad;
    if(nullptr != mProcess)   delete mProcess;
    if(nullptr != ui)         delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton answer = QMessageBox::question(
    this,
    tr("Close SILClientDL?"),
    tr("Do you really want to close the SILClientDL?"),
    QMessageBox::Yes | QMessageBox::No);
    if (answer == QMessageBox::Yes)
    {
        event->accept();
        if(isProcessRunning())
           mProcess->kill();
    }
    else
        event->ignore();
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
            {
                this->ui->LoadAnimation->hide();
            }
            mProcess->terminate();
            if(warningMessage(msg))
                return;
        }
        mMovieLoad->start();
        setEnabledBtn(false);
    }
    this->ui->LoadAnimation->show();
}

void MainWindow::on_btnShutdown_clicked()
{
    if(true == isProcessRunning())
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<eProcessCmd>();
        writeToStdin(metaEnum.valueToKey(SIL_SHUTDOWN));
    }
    mMovieLoad->setPaused(true);
    setEnabledBtn(true);
}

void MainWindow::on_actionPath_to_a_binary_triggered()
{
    mBinaryPath = QFileDialog::getOpenFileName(this,"Select "
                         "file", "/home", "All files (*.*)");
    if(!mBinaryPath.isEmpty())
    {
        mMovieLoad->start();
        mMovieLoad->stop();
        if(this->ui->LoadAnimation->isHidden())
        {
            this->ui->LoadAnimation->show();
        }
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
    QString text = QInputDialog::getText(0, "", "Run Period(ms):"
                 , QLineEdit::Normal, mRunPeriod, &ok);
    if(!ok || text.isEmpty())
        return;
    if(minPeriod > text.toInt() || maxPeriod < text.toInt())
    {
        static_cast<void>(warningMessage
        ("The running period should be in the range(20 - 200)"));
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
        QByteArray array = text.toLocal8Bit();

        static_cast<void>(mProcess->write(array.data()));
        static_cast<void>(mProcess->waitForBytesWritten());
        mProcess->closeWriteChannel();
    }
}

bool MainWindow::isProcessRunning() const
{
    return QProcess::NotRunning == mProcess->state()
           ? false : true;
}

void MainWindow::setLabelIcon(QLabel* label, const QString& path)
{
    label->setPixmap(QPixmap(path));
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
    QString info
    ("This app was developed to have the ability to test Data Logger."
     "If you are brave developer and open to this chalange you can "
     "choose a binary, set up a value of cycle and enjoy it");
    QMessageBox::information(this, "Info", info);
}
