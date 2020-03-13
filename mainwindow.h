#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QProcess;
class QMovie;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum eProcessCmd { SIL_UNKNOWN = 0, SIL_SHUTDOWN};
    Q_ENUM(eProcessCmd)

    MainWindow(QWidget* = nullptr);
    ~MainWindow();

protected:
    void closeEvent    (QCloseEvent*) override;

private:
    bool warningMessage(const QString&);
    void writeToStdin(const QString &);
    bool isProcessRunning() const;
    void setLabelIcon(QLabel*, const QString&);
    void setEnabledBtn(bool);

private slots:
    void on_btnRun_clicked();
    void on_btnShutdown_clicked();
    void on_actionPath_to_a_binary_triggered();
    void on_actionSet_Run_period_triggered();
    void on_actionSave_to_File_triggered();
    void on_btnClear_clicked();
    void on_btnInfo_clicked();

private:
    Ui::MainWindow *ui;

    QMovie*   mMovieLoad;
    QIcon     mClear;
    QProcess* mProcess;
    QString   mBinaryPath;
    QString   mRunPeriod;
    bool      isRuning;

    const int minPeriod =  20;
    const int maxPeriod = 255;
};
#endif // MAINWINDOW_H
