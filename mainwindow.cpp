#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include "tracesarea.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QMetaEnum>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mMovieLoad(QString( ":/icons/ajax-loader.gif"))
    , mProcess(new QProcess(parent))
    , mBinaryPath("")
    , mRunPeriod("40")
    , isRuning(false)
{
    ui->setupUi(this);
    auto mUi =  this->ui;
    mMovieLoad.setScaledSize(mUi->LoadAnimation->size());
    mUi->LoadAnimation->setMovie(&mMovieLoad);
    //mMovieLoad.start();
    //.stop ();

   // setLabelIcon(mUi->labelClear, ":/icons/clean_new.png"   );
   // setLabelIcon(mUi->Shutdown,   ":/icons/power_off_2.png");
    setLabelIcon(mUi->labelFrog,  ":/icons/frog.png"    );

    connect(mProcess, &QProcess::readyReadStandardOutput, [this](){
        QString output = mProcess->readAllStandardOutput();
        this->ui->tracesArea->outputMessage(QtInfoMsg, QMessageLogContext(), output);
    });
    connect(mProcess, &QProcess::readyReadStandardError, [this](){
        QString err = mProcess->readAllStandardError();
        this->ui->tracesArea->outputMessage(QtCriticalMsg, QMessageLogContext(), err);
    });
}

MainWindow::~MainWindow()
{
    if(isProcessRunning())
        mProcess->kill();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    static_cast<void>(event);
    if(isProcessRunning())
    {
        mProcess->terminate();
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
           // this->ui->LoadAnimation->clear();
            if(warningMessage(msg))
                return;
        }
        else
        {
            qDebug() << "Running!!!";
            //this->ui->LoadAnimation->setMovie(&mMovieLoad);
            mMovieLoad.start();
           // setLabelIcon(this->ui->Shutdown, ":/icons/power_on.png");
            isRuning = true;
        }
    }
    this->ui->LoadAnimation->show();
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

    mMovieLoad.setPaused(true);
    isRuning = false;
}

void MainWindow::on_actionPath_to_a_binary_triggered()
{
    mBinaryPath = QFileDialog::getOpenFileName(this,
                                         "Select binary file",
                                         "/home",
                                         "All files (*.*)");
    if(!mBinaryPath.isEmpty())
    {
        mMovieLoad.start();
        mMovieLoad.stop();
    }
    //stop process!!! TODO
        //this->ui->LoadAnimation->hide();
    //show
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
        this->ui->labelPeriodValue->setText(text);
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
