#pragma once

#include <nlohmann/json.hpp>

#include <QHotkey>
#include <Windows.h>

using json = nlohmann::json;

class MacroHandler : public QObject
{
    Q_OBJECT

public:
    MacroHandler(QObject* parent = nullptr);

    void setMacros(json macrolist);
    void clearMacros();

public slots:
    void handleForegroundChange(bool isPoe);

private:
    void insertKeyPress(std::vector<INPUT>& keystrokes, WORD key);
    void insertChatCommand(std::vector<INPUT>& keystrokes, std::string command);
    void sendChatCommand(std::string command);

private slots:
    void handleMacro(QString key);

private:
    json                                  m_macrolist;
    std::vector<std::unique_ptr<QHotkey>> m_macros;
};