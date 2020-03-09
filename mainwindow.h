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
    bool warrningMessage(const QString& msg);

private slots:

    void on_Run_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_btnRun_clicked();

    void on_btnShutdown_clicked();

    void on_actionPath_to_a_binary_triggered();

private:
    Ui::MainWindow *ui;
    QMovie  mMovieLoad;
    QMovie  mMovieInitS;
    QMovie  mMovieShutdown;

    QProcess* mProcess;
    QString   mBinaryPath;
};
#endif // MAINWINDOW_H
