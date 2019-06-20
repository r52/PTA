#include "logwindow.h"

#include "pta_types.h"

#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QTextEdit>
#include <QTextStream>

#include <mutex>

/*
 * This is a fake singleton unique pointer hybrid monster thing
 * designed to work around the fact that Qt widget parents take
 * ownership of added child widgets, so that the lifetime of
 * the logedit widget can be maximized and ensure proper clean up
 */

namespace
{
    std::mutex            s_logMutex;
    QTextEdit*            s_logEdit = nullptr;
    QScopedPointer<QFile> s_logFile;
}

void open_log_file()
{
    QString path       = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString logFileExt = ".log";

    s_logFile.reset(new QFile(path + logFileExt));
    s_logFile->open(QFile::Append | QFile::Text);

    // 1MB limit
    if (s_logFile->size() > 1048576)
    {
        size_t logidx = 1;
        while (false == s_logFile->rename(path + logFileExt + QString(".%1").arg(QString::number(logidx))))
        {
            logidx++;
        }

        s_logFile.reset(new QFile(path + logFileExt));
        s_logFile->open(QFile::Append | QFile::Text);
    }
}

void msg_handler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QSettings setting;
    int       loglevel = setting.value(PTA_CONFIG_LOGLEVEL, PTA_CONFIG_DEFAULT_LOGLEVEL).toInt();
    bool      logFile  = setting.value(PTA_CONFIG_LOGFILE, false).toBool();
    bool      print    = false;

    switch (type)
    {
        case QtDebugMsg:
            print = (loglevel == PTA_LOG_DEBUG);
            break;

        case QtInfoMsg:
            print = (loglevel <= PTA_LOG_INFO);
            break;

        case QtWarningMsg:
            print = (loglevel <= PTA_LOG_WARNING);
            break;

        case QtCriticalMsg:
            print = (loglevel <= PTA_LOG_CRITICAL);
            break;

        default:
            print = true;
            break;
    }

    if (print)
    {
        std::unique_lock<std::mutex> lock(s_logMutex);

        QString output = qFormatLogMessage(type, context, msg);

        if (s_logEdit)
        {
            s_logEdit->append(output);
        }

        if (logFile && !s_logFile)
        {
            open_log_file();
        }

        if (s_logFile)
        {
            QTextStream out(s_logFile.data());
            out << output << endl;
            out.flush();
        }
    }
}

LogWindow::~LogWindow()
{
    // if released, s_logEdit is not owned anymore
    // otherwise it needs to be cleaned
    if (!m_released && nullptr != s_logEdit)
    {
        delete s_logEdit;
    }

    s_logEdit = nullptr;
}

LogWindow::LogWindow(QObject* parent) : QObject(parent)
{
    if (nullptr != s_logEdit)
    {
        throw std::runtime_error("log window already created");
    }

    s_logEdit = new QTextEdit();

    s_logEdit->setReadOnly(true);
    s_logEdit->setAcceptRichText(true);

    // max lines in log viewer
    s_logEdit->document()->setMaximumBlockCount(250);

    qInstallMessageHandler(msg_handler);
    qSetMessagePattern("[%{time}]    %{type}    %{message} - (%{function}:%{line})");
}

QTextEdit* LogWindow::release()
{
    if (nullptr != s_logEdit)
    {
        m_released = true;
    }

    return s_logEdit;
}
