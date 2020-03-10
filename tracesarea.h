#ifndef TRACESAREA_H
#define TRACESAREA_H

#include <QTextBrowser>


class TracesArea : public QTextBrowser
{
    Q_OBJECT
public:
    explicit TracesArea(QWidget *parent = nullptr);
    void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    TracesArea* self();


signals:
 //void sendMessage( QtMsgType type, const QString &msg );

private:
 //LogBrowserDialog *browserDialog;

};

inline TracesArea* TracesArea::self()
{
    return this;
}

#endif // TRACESAREA_H
