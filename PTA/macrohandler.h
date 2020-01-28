#pragma once

#include <nlohmann/json.hpp>

#include <functional>

#include <QHotkey>
#include <Windows.h>

using json = nlohmann::json;

class ClientMonitor;

class MacroHandler : public QObject
{
    Q_OBJECT

public:
    MacroHandler(ClientMonitor* client, QObject* parent = nullptr);

    void setMacros(json macrolist);
    void clearMacros();

signals:
    void humour(const QString& msg);

public slots:
    void handleForegroundChange(bool isPoe);

private:
    void insertKeyPress(std::vector<INPUT>& keystrokes, WORD key);
    void insertChatCommand(std::vector<INPUT>& keystrokes, std::string command);
    bool processChatCommand(std::string& command);
    void sendChatCommand(std::string command);

private slots:
    void handleMacro(QString key);

private:
    QMap<QString, std::function<QString(void)>> m_variables;

    ClientMonitor* m_client;

    json                                  m_macrolist;
    std::vector<std::unique_ptr<QHotkey>> m_macros;
};