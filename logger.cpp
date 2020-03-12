#include "logger.h"


#include <QMetaType>
#include "logbrowserdialog.h"
#include <QSizePolicy>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>


Logger::Logger(QObject *parent) : QObject(parent)
{

}


void Logger::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

}

void Logger::saveLogsToFileSytem()
{

}








void Logger::process()
{
    emit finished();
}

void Logger::stop()
{

}
