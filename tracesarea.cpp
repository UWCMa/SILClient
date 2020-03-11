#include "tracesarea.h"
#include <QMetaType>
#include "logbrowserdialog.h"
#include <QSizePolicy>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

TracesArea::TracesArea(QWidget *parent)
    : QTextBrowser(parent)
{
}

void TracesArea::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString str = msg;
    switch (type) {
    case QtDebugMsg:
       // fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        append(tr("— DDEBUG: %1").arg(str));
        break;
    case QtInfoMsg:
        //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        append(tr("— INFO: %1").arg(str));
        break;
    case QtWarningMsg:
        //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        append(tr("— WARNING: %1").arg(str));
        break;
    case QtCriticalMsg:
        //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        append(tr("— CRITICAL: %1").arg(str));
        break;
    case QtFatalMsg:
        //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        append(tr("— FATAL: %1").arg(str));
        break;
    }
}

void TracesArea::saveLogsToFileSstem()
{
    QString saveFileName = QFileDialog::getSaveFileName(
                           this,
                           tr("Save Log Output"),
                           tr("%1/logfile.txt").arg(QDir::homePath()),
                           tr("Text Files (*.txt);;All Files (*)"));
    if(saveFileName.isEmpty())
        return;

    QFile file(saveFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(
                 this,
                 tr("Error"),
                 QString(tr("<nobr>File '%1'<br/>cannot be opened for writing.<br/><br/>"
                 "The log output could <b>not</b> be saved!</nobr>"))
                 .arg(saveFileName));
    return;
    }
    QTextStream stream(&file);
    stream << toPlainText();
    file.close();
}
