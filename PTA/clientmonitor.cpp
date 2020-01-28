#include "clientmonitor.h"

#include "pta_types.h"

#include <QDebug>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSettings>
#include <QTimer>

using namespace std::chrono_literals;

const std::chrono::milliseconds ClientMonitor::polling_rate = 350ms;

ClientMonitor::ClientMonitor(QObject* parent) : QObject(parent), m_enabled(false), m_last_whisper(QString())
{
    QSettings settings;

    QString logpath = settings.value(PTA_CONFIG_CLIENTLOG_PATH, QString()).toString();

    if (logpath.isEmpty())
    {
        qInfo() << "Client.txt path not set. Client features disabled.";
    }
    else
    {
        setPath(logpath);
    }
}

ClientMonitor::~ClientMonitor()
{
    m_watcher.reset();
}

void ClientMonitor::setPath(QString logpath)
{
    if (logpath.isEmpty())
    {
        // If empty path, disable monitor
        m_enabled = false;

        // Clear file watcher
        if (m_watcher)
        {
            m_watcher.reset();
        }

        m_logpath.clear();

        return;
    }

    if (!QFileInfo::exists(logpath))
    {
        qWarning() << "Client log file not found at" << logpath;
        return;
    }

    if (m_logpath == logpath)
    {
        // Same file
        return;
    }

    m_logpath = logpath;

    // Pre-read size
    QFile log(m_logpath);

    if (!log.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot process client log file found at" << m_logpath;
        return;
    }

    m_lastpos = log.size();

    log.close();

    m_watcher.reset(new QTimer(this));

    connect(m_watcher.get(), &QTimer::timeout, this, &ClientMonitor::processLogChange);
    m_watcher->start(polling_rate);

    m_enabled = true;

    qInfo() << "Client.txt set to" << m_logpath;
}

bool ClientMonitor::enabled()
{
    return m_enabled;
}

QString ClientMonitor::getLastWhisperer()
{
    return m_last_whisper;
}

void ClientMonitor::processLogLine(QString line)
{
    auto parts = line.splitRef("] ", QString::SkipEmptyParts);

    if (parts.size() < 2)
    {
        // If not a game info line, skip
        return;
    }

    // Get last part
    auto ltxt = parts[parts.size() - 1].trimmed().toString();

    if (ltxt.startsWith('@'))
    {
        // Whisper

        // Remove whisper tags
        ltxt.remove(QRegularExpression("^@(From|To) (<\\S+> )?"));

        auto msgparts = ltxt.splitRef(": ", QString::SkipEmptyParts);

        if (msgparts.size() < 1)
        {
            qWarning() << "Error parsing whisper text:" << ltxt;
            return;
        }

        auto pname = msgparts[0].toString();

        m_last_whisper = pname;
    }
}

void ClientMonitor::processLogChange()
{
    if (!enabled())
    {
        // Shouldn't ever get here
        return;
    }

    QFile log(m_logpath);

    if (!log.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot process client log file found at" << m_logpath;
        return;
    }

    qint64 lastpos = log.size();

    if (lastpos < m_lastpos)
    {
        // File got reset, so reset our position too
        m_lastpos = lastpos;
        return;
    }

    if (lastpos == m_lastpos)
    {
        // No change
        return;
    }

    // Start reading from
    log.seek(m_lastpos);

    QTextStream in(&log);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        processLogLine(line);
    }

    m_lastpos = lastpos;

    log.close();
}
