#include "tracesarea.h"
#include "logbrowserdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

TracesArea::TracesArea(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setReadOnly(true);
}

void TracesArea::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static_cast<void>(context);
    switch (type) {
    case QtDebugMsg:
       // fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        appendPlainText(tr("— DDEBUG: %1").arg(msg));
        break;
    case QtInfoMsg:
        //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        appendPlainText(tr("— INFO: %1").arg(msg));
        break;
    case QtWarningMsg:
        //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        appendPlainText(tr("— WARNING: %1").arg(msg));
        break;
    case QtCriticalMsg:
        //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        appendPlainText(tr("— CRITICAL: %1").arg(msg));
        break;
    case QtFatalMsg:
        //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        appendPlainText(tr("— FATAL: %1").arg(msg));
        break;
    default:
        break;
    }
}

void TracesArea::saveLogsToFileSytem()
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
