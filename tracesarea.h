#ifndef TRACESAREA_H
#define TRACESAREA_H

#include <QTextBrowser>

class TracesArea : public QTextBrowser
{
    Q_OBJECT

public:
    explicit TracesArea(QWidget * = nullptr);
    void outputMessage(QtMsgType, const QMessageLogContext&
                                , const QString &);
    void saveLogsToFileSytem();
};
#endif // TRACESAREA_H
