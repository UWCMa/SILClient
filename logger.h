#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);

    void outputMessage(QtMsgType, const QMessageLogContext&
                                , const QString &);
    void saveLogsToFileSytem();


public slots:
    void process(); 	/*  создает и запускает построитель отчетов */
    void stop();
signals:
    void finished(); 	/* сигнал о завершении  работы построителя отчетов */

};

#endif // LOGGER_H
