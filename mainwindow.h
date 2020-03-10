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

    enum eProcessCmd { SIL_UNKNOWN = 0, SIL_SHUTDOWN};
    Q_ENUM(eProcessCmd)

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent (QCloseEvent *event);

private:
    bool warningMessage(const QString& msg);
    void writeToStdin(const QString &text);
    bool isProcessRunning() const;

private slots:

    void on_Run_clicked();


    void on_btnRun_clicked();

    void on_btnShutdown_clicked();

    void on_actionPath_to_a_binary_triggered();

    void on_actionSet_Run_period_triggered();

    void on_actionSave_to_File_triggered();

    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;
    QMovie  mMovieLoad;
    QMovie  mMovieInitS;
    QMovie  mMovieShutdown;
    QIcon   mClear;

    QProcess* mProcess;
    QString   mBinaryPath;
    QString   mRunPeriod;
    bool      isRuning;

};
#endif // MAINWINDOW_H
