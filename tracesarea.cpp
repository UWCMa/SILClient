#include "tracesarea.h"
#include <QMetaType>
#include "logbrowserdialog.h"
#include <iostream>
#include <QSizePolicy>

TracesArea::TracesArea(QWidget *parent) :QTextBrowser(parent)
{
    std::cout << "TracesArea::TracesArea" << std::endl;
}

void TracesArea::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
 /*switch (type) {
 case QtDebugMsg:
 append(msg);
 break;

 case QtWarningMsg:
 append(tr("— WARNING: %1").arg(msg));
 break;

 case QtCriticalMsg:
 append(tr("— CRITICAL: %1").arg(msg));
 break;

 case QtFatalMsg:
 append(tr("— FATAL: %1").arg(msg));
 break;
 }*/

    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    QString str = msg;
    str.append(" ");
    str.append(function);
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        append(tr("— INFO: %1").arg(str));
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        append(tr("— WARNING: %1").arg(str));
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    }
}
