#pragma once

#include <chrono>

#include <QObject>

class QTimer;

class ClientMonitor : public QObject
{
    Q_OBJECT

public:
    ClientMonitor(QObject* parent = nullptr);
    ~ClientMonitor();

    void setPath(QString logpath);

    bool enabled();

    QString getLastWhisperer();

private slots:
    void processLogChange();
    void processLogLine(QString line);

private:
    static const std::chrono::milliseconds polling_rate;

    // We are FORCED to use a polling technique for Client.txt because it doesn't flush AT ALL
    // unless the file is accessed externally. This renders functions like QFileSystemWatcher and
    // even the WinAPI ReadDirectoryChangesW UNUSABLE for our purposes :(.
    std::unique_ptr<QTimer> m_watcher;

    QString m_logpath;

    bool   m_enabled;
    qint64 m_lastpos = -1;

    QString m_last_whisper;
};