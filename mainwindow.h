#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include <QProcess>


class TracesArea;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool warningMessage(const QString& msg);

private slots:

    void on_Run_clicked();


    void on_btnRun_clicked();

    void on_btnShutdown_clicked();

    void on_actionPath_to_a_binary_triggered();

    void on_actionSet_Run_period_triggered();

private:
    Ui::MainWindow *ui;
    QMovie  mMovieLoad;
    QMovie  mMovieInitS;
    QMovie  mMovieShutdown;
    QIcon  mClear;

    QProcess* mProcess;
    QString   mBinaryPath;

    bool isRuning;
    int  mRunPeriod;
};
#endif // MAINWINDOW_H