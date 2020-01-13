#include "macrohandler.h"

#include "pta_types.h"
#include "putil.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QSettings>
#include <QUrl>

MacroHandler::MacroHandler(QObject* parent) : QObject(parent)
{
    QSettings settings;

    auto macstr = settings.value(PTA_CONFIG_CUSTOM_MACROS).toString().toStdString();

    if (!macstr.empty())
    {
        auto macrolist = json::parse(macstr);

        setMacros(macrolist);
    }

    m_monitor = new QTimer(this);
    connect(m_monitor, &QTimer::timeout, this, &MacroHandler::monitorPoEForeground);
    m_monitor->start(500);
}

void MacroHandler::setMacros(json macrolist)
{
    m_macros.clear();

    m_macrolist = macrolist;

    for (auto& [k, v] : macrolist.items())
    {
        auto key = QString::fromStdString(k);
        auto seq = QString::fromStdString(v["sequence"].get<std::string>());

        std::unique_ptr<QHotkey> macro(new QHotkey(QKeySequence(seq), true));

        if (!macro->isRegistered())
        {
            qWarning() << "Macro" << key << "failed to register";
        }

        qDebug() << "Macro" << key << "Registered:" << macro->isRegistered();

        connect(macro.get(), &QHotkey::activated, [=]() { handleMacro(key); });

        m_macros.push_back(std::move(macro));
    }
}

void MacroHandler::clearMacros()
{
    m_macros.clear();
}

void MacroHandler::insertKeyPress(std::vector<INPUT>& keystrokes, WORD key)
{
    keystrokes.push_back(pta::CreateInput(key, true));
    keystrokes.push_back(pta::CreateInput(key, false));
}

void MacroHandler::insertChatCommand(std::vector<INPUT>& keystrokes, std::string command)
{
    QGuiApplication::clipboard()->setText(QString::fromStdString(command));

    // ensure all used keys are up
    keystrokes.push_back(pta::CreateInput(VK_MENU, false));
    keystrokes.push_back(pta::CreateInput(VK_CONTROL, false));
    keystrokes.push_back(pta::CreateInput('V', false));
    keystrokes.push_back(pta::CreateInput('A', false));
    keystrokes.push_back(pta::CreateInput(VK_BACK, false));

    // open chat
    insertKeyPress(keystrokes, VK_RETURN);

    // select all and delete
    keystrokes.push_back(pta::CreateInput(VK_CONTROL, true));
    keystrokes.push_back(pta::CreateInput('A', true));
    keystrokes.push_back(pta::CreateInput('A', false));
    keystrokes.push_back(pta::CreateInput(VK_CONTROL, false));
    insertKeyPress(keystrokes, VK_BACK);

    // paste command
    keystrokes.push_back(pta::CreateInput(VK_CONTROL, true));
    keystrokes.push_back(pta::CreateInput('V', true));
    keystrokes.push_back(pta::CreateInput('V', false));
    keystrokes.push_back(pta::CreateInput(VK_CONTROL, false));

    // press enter
    insertKeyPress(keystrokes, VK_RETURN);
}

void MacroHandler::sendChatCommand(std::string command)
{
    std::vector<INPUT> keystroke;

    insertChatCommand(keystroke, command);

    // Send input
    SendInput(keystroke.size(), keystroke.data(), sizeof(keystroke[0]));
}

void MacroHandler::monitorPoEForeground()
{
    // Gotta do this because hotkeys are consumed by the app :(

    bool enabled = true;

    if (!pta::IsPoEForeground())
        enabled = false;

    for (auto& macro : m_macros)
    {
        macro->setRegistered(enabled);
    }
}

void MacroHandler::handleMacro(QString key)
{
    if (!pta::IsPoEForeground())
        return;

    auto skey = key.toStdString();

    if (!m_macrolist.contains(skey))
    {
        qWarning() << "Macro" << key << "not found.";
        return;
    }

    auto& macro = m_macrolist[skey];

    int  type    = macro["type"].get<int>();
    auto command = macro["command"].get<std::string>();

    switch (type)
    {
        case MACRO_TYPE_CHAT:
        {
            sendChatCommand(command);
            break;
        }

        case MACRO_TYPE_URL:
        {
            QDesktopServices::openUrl(QUrl(QString::fromStdString(command)));
            break;
        }

        default:
        {
            qWarning() << "Invalid macro type:" << type;
            return;
        }
    }
}
