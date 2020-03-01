#include "pta.h"

#include "configdialog.h"
#include "itemapi.h"
#include "logwindow.h"
#include "pta_types.h"
#include "putil.h"
#include "version.h"
#include "webwidget.h"

#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QPlainTextEdit>
#include <QToolTip>
#include <QVBoxLayout>

#include <Windows.h>

namespace
{
    bool g_ctrlScrollEnabled = false;
}

PTA::PTA(LogWindow* log, QWidget* parent) :
    QMainWindow(parent),
    m_logWindow(log),
    m_inputhandler(this),
    m_clientmonitor(this),
    m_macrohandler(&m_clientmonitor, this),
    m_netmanager(new QNetworkAccessManager(this))
{
    if (nullptr == m_logWindow)
    {
        throw std::invalid_argument("Invalid LogWindow");
    }

    m_logWindow->setParent(this);

    ui.setupUi(this);

    // Try to initialize API

    // Enable data caching for faster loads
    QNetworkDiskCache* diskCache = new QNetworkDiskCache(this);

    auto cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

    diskCache->setCacheDirectory(cachePath);
    m_netmanager->setCache(diskCache);

    try
    {
        m_api = new ItemAPI(m_netmanager, this);
    } catch (std::exception e)
    {
        QMessageBox::critical(nullptr, tr("Critical Error"), QString(e.what()), QMessageBox::Abort);
        qApp->exit(1);
    }

    // Setup system tray
    createActions();
    createTrayIcon();

    // Setup icon
    QIcon icon(":/Resources/logo.svg");
    setWindowIcon(icon);
    m_trayIcon->setIcon(icon);

    m_trayIcon->show();

    // Setup log window
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_logWindow->release());

    ui.centralWidget->setLayout(layout);

    resize(800, 400);

    using namespace std::placeholders;

    // Setup functionality
    setupFunctionality();

#ifndef _DEBUG
    // Initialize hooks
    pta::hook::InstallForegroundHookCb(std::bind(&PTA::foregroundEventCb, this, _1));
    pta::hook::InitializeHooks();
#endif

    checkForUpdates();
}

void PTA::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::Context:
        {
            if (reason == QSystemTrayIcon::Trigger)
            {
                m_trayIcon->contextMenu()->popup(QCursor::pos());
            }

            break;
        }
    }
}

PTA::~PTA()
{
    m_simpleKey.reset();
    m_advancedKey.reset();
    m_wikiKey.reset();

    pta::hook::ShutdownHooks();
}

void PTA::showToolTip(QString message)
{
    QToolTip::showText(QCursor::pos() + QPoint(5, 20), message);
}

void PTA::showPriceWidget(const QString& data)
{
    showToolTip(QString());

    auto pricedlg = new WebWidget(m_api, data);
    pricedlg->show();
}

void PTA::closeEvent(QCloseEvent* event)
{
    if (m_trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void PTA::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_settingsAction);
    m_trayIconMenu->addAction(m_logAction);
    m_trayIconMenu->addAction(m_suspendAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_aboutAction);
    m_trayIconMenu->addAction(m_aboutQtAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &PTA::trayIconActivated);
}

void PTA::createActions()
{
    m_settingsAction = new QAction(tr("&Settings"), this);
    connect(m_settingsAction, &QAction::triggered, this, &PTA::openSettings);

    m_logAction = new QAction(tr("L&og"), this);
    connect(m_logAction, &QAction::triggered, this, &QWidget::showNormal);

    m_suspendAction = new QAction(tr("Sus&pend Hotkeys"), this);
    m_suspendAction->setCheckable(true);
    m_suspendAction->setChecked(m_blockHotkeys);
    connect(m_suspendAction, &QAction::triggered, [&](bool checked) { m_blockHotkeys = checked; });

    m_aboutAction = new QAction(tr("&About PTA"), this);

    connect(m_aboutAction, &QAction::triggered, [&] {
        static QString aboutMsg = "PTA " VER_STRING "<br/>"
                                  "<br/>"
                                  "Compiled on: " __DATE__ "<br/>"
                                  "Qt version: " QT_VERSION_STR "<br/>"
                                  "<br/>"
                                  "Licensed under GPLv3<br/>"
                                  "Source code available at <a href='https://github.com/r52/PTA'>GitHub</a><br/>"
                                  "Icons from <a href='https://icons8.com'>https://icons8.com</a>";

        QMessageBox::about(this, tr("About PTA"), aboutMsg);
    });

    m_aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(m_aboutQtAction, &QAction::triggered, [&] { QMessageBox::aboutQt(this, "About Qt"); });

    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void PTA::setupFunctionality()
{
    // UI
    connect(m_api, &ItemAPI::humour, this, &PTA::showToolTip);
    connect(m_api, &ItemAPI::openSearchUI, this, &PTA::showPriceWidget);

    connect(&m_macrohandler, &MacroHandler::humour, this, &PTA::showToolTip);

    // Hotkeys
    QSettings settings;

    bool simpleEnabled = settings.value(PTA_CONFIG_SIMPLE_CHECK_HOTKEY_ENABLED, true).toBool();

    if (simpleEnabled)
    {
        QString seq = settings.value(PTA_CONFIG_SIMPLE_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_SIMPLE_CHECK_HOTKEY).toString();

        m_simpleKey.reset(new QHotkey(QKeySequence(seq), true));
        qDebug() << "Price Check Hotkey Registered:" << m_simpleKey->isRegistered();

        connect(m_simpleKey.get(), &QHotkey::activated, [=]() { handleItemHotkey(PC_SIMPLE); });
    }

    bool advEnabled = settings.value(PTA_CONFIG_ADV_CHECK_HOTKEY_ENABLED, true).toBool();

    if (advEnabled)
    {
        QString seq = settings.value(PTA_CONFIG_ADV_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_ADV_CHECK_HOTKEY).toString();

        m_advancedKey.reset(new QHotkey(QKeySequence(seq), true));
        qDebug() << "Advanced Price Check Hotkey Registered:" << m_advancedKey->isRegistered();

        connect(m_advancedKey.get(), &QHotkey::activated, [=]() { handleItemHotkey(PC_ADVANCED); });
    }

    bool wikiEnabled = settings.value(PTA_CONFIG_WIKI_HOTKEY_ENABLED, true).toBool();

    if (wikiEnabled)
    {
        QString seq = settings.value(PTA_CONFIG_WIKI_HOTKEY, PTA_CONFIG_DEFAULT_WIKI_HOTKEY).toString();

        m_wikiKey.reset(new QHotkey(QKeySequence(seq), true));
        qDebug() << "Wiki Search Hotkey Registered:" << m_wikiKey->isRegistered();

        connect(m_wikiKey.get(), &QHotkey::activated, [=]() { handleItemHotkey(WIKI_SEARCH); });
    }

    // ctrl + scroll
    g_ctrlScrollEnabled = settings.value(PTA_CONFIG_CTRL_SCROLL_HOTKEY_ENABLED, true).toBool();

    // Clipboard
    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &PTA::handleClipboard);

    // Foreground change events
    connect(this, &PTA::foregroundWindowChanged, &m_macrohandler, &MacroHandler::handleForegroundChange);
    connect(this, &PTA::foregroundWindowChanged, this, &PTA::handleForegroundChange);
}

void PTA::checkForUpdates()
{
    QTimer::singleShot(5000, [&] {
        m_updrequest.setUrl(QUrl("https://api.github.com/repos/r52/PTA/releases"));
        auto reply = m_netmanager->get(m_updrequest);
        connect(reply, &QNetworkReply::finished, [=]() { processUpdates(reply); });
    });
}

void PTA::foregroundEventCb(bool isPoe)
{
    emit foregroundWindowChanged(isPoe);
}

void PTA::openSettings()
{
    if (!m_configdialog)
    {
        m_configdialog = new ConfigDialog(m_api);

        connect(m_configdialog, &QDialog::finished, this, &PTA::saveSettings);

        m_configdialog->open();
    }
}

void PTA::saveSettings(int result)
{
    ConfigDialog* dlg = m_configdialog;
    m_configdialog    = nullptr;

    if (result == QDialog::Rejected)
    {
        // cancelled
        return;
    }

    json results = dlg->results;

    QSettings settings;

    for (const auto& [k, v] : results.items())
    {
        if (v.is_boolean())
        {
            settings.setValue(QString::fromStdString(k), v.get<bool>());
        }
        else if (v.is_number())
        {
            settings.setValue(QString::fromStdString(k), v.get<int>());
        }
        else if (v.is_string())
        {
            settings.setValue(QString::fromStdString(k), QString::fromStdString(v.get<std::string>()));
        }

        if (k == PTA_CONFIG_SIMPLE_CHECK_HOTKEY_ENABLED)
        {
            bool enabled = v.get<bool>();

            if (!enabled)
            {
                m_simpleKey.reset();
            }
            else if (!m_simpleKey)
            {
                QString seq = settings.value(PTA_CONFIG_SIMPLE_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_SIMPLE_CHECK_HOTKEY).toString();

                m_simpleKey.reset(new QHotkey(QKeySequence(seq), true));
                qDebug() << "Price Check Hotkey Registered:" << m_simpleKey->isRegistered();

                connect(m_simpleKey.get(), &QHotkey::activated, [=]() { handleItemHotkey(PC_SIMPLE); });
            }
        }

        if (k == PTA_CONFIG_ADV_CHECK_HOTKEY_ENABLED)
        {
            bool enabled = v.get<bool>();

            if (!enabled)
            {
                m_advancedKey.reset();
            }
            else if (!m_advancedKey)
            {
                QString seq = settings.value(PTA_CONFIG_ADV_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_ADV_CHECK_HOTKEY).toString();

                m_advancedKey.reset(new QHotkey(QKeySequence(seq), true));
                qDebug() << "Advanced Price Check Hotkey Registered:" << m_advancedKey->isRegistered();

                connect(m_advancedKey.get(), &QHotkey::activated, [=]() { handleItemHotkey(PC_ADVANCED); });
            }
        }

        if (k == PTA_CONFIG_WIKI_HOTKEY_ENABLED)
        {
            bool enabled = v.get<bool>();

            if (!enabled)
            {
                m_wikiKey.reset();
            }
            else if (!m_wikiKey)
            {
                QString seq = settings.value(PTA_CONFIG_WIKI_HOTKEY, PTA_CONFIG_DEFAULT_WIKI_HOTKEY).toString();

                m_wikiKey.reset(new QHotkey(QKeySequence(seq), true));
                qDebug() << "Wiki Hotkey Registered:" << m_advancedKey->isRegistered();

                connect(m_wikiKey.get(), &QHotkey::activated, [=]() { handleItemHotkey(WIKI_SEARCH); });
            }
        }

        if (k == PTA_CONFIG_SIMPLE_CHECK_HOTKEY)
        {
            QString nseq = QString::fromStdString(v.get<std::string>());

            if (m_simpleKey)
            {
                QString currseq = m_simpleKey->shortcut().toString();

                if (nseq != currseq)
                {
                    m_simpleKey->setShortcut(QKeySequence(nseq), true);
                    qDebug() << "Price Check Hotkey Registered:" << m_simpleKey->isRegistered();
                }
            }
        }

        if (k == PTA_CONFIG_ADV_CHECK_HOTKEY)
        {
            QString nseq = QString::fromStdString(v.get<std::string>());

            if (m_advancedKey)
            {
                QString currseq = m_advancedKey->shortcut().toString();

                if (nseq != currseq)
                {
                    m_advancedKey->setShortcut(QKeySequence(nseq), true);
                    qDebug() << "Advanced Price Check Hotkey Registered:" << m_advancedKey->isRegistered();
                }
            }
        }

        if (k == PTA_CONFIG_WIKI_HOTKEY)
        {
            QString nseq = QString::fromStdString(v.get<std::string>());

            if (m_wikiKey)
            {
                QString currseq = m_wikiKey->shortcut().toString();

                if (nseq != currseq)
                {
                    m_wikiKey->setShortcut(QKeySequence(nseq), true);
                    qDebug() << "Wiki Hotkey Registered:" << m_wikiKey->isRegistered();
                }
            }
        }

        if (k == PTA_CONFIG_CTRL_SCROLL_HOTKEY_ENABLED)
        {
            g_ctrlScrollEnabled = v.get<bool>();
        }

        if (k == PTA_CONFIG_CUSTOM_MACROS)
        {
            auto mcs = v.dump();

            settings.setValue(PTA_CONFIG_CUSTOM_MACROS, QString::fromStdString(mcs));

            m_macrohandler.setMacros(v);
        }

        if (k == PTA_CONFIG_CLIENTLOG_PATH)
        {
            QString logpath = QString::fromStdString(v.get<std::string>());

            m_clientmonitor.setPath(logpath);
        }
    }
}

void PTA::handleScrollHotkey(short data)
{
    WORD key = (data > 0 ? VK_LEFT : VK_RIGHT);

    // Send input
    std::vector<INPUT> keystroke;

    keystroke.push_back(pta::CreateInput(key, true));
    keystroke.push_back(pta::CreateInput(key, false));

    SendInput(keystroke.size(), keystroke.data(), sizeof(keystroke[0]));
}

void PTA::handleItemHotkey(uint32_t flag)
{
    if (m_blockHotkeys)
    {
        // Currently blocked
        return;
    }

    if (m_pcTriggered)
    {
        // Another search triggered
        return;
    }

    m_blockHotkeys = true;

    // Check for PoE window
    if (!pta::IsPoEForeground())
    {
        m_blockHotkeys = false;
        return;
    }

    // Arm trigger
    m_pctype      = flag;
    m_pcTriggered = true;

    QTimer::singleShot(100, [=]() {
        // Reset trigger if still armed
        if (m_pcTriggered)
        {
            m_pcTriggered = false;
        }
    });

    // Send ctrl-c
    std::vector<INPUT> keystroke;

    // ensure ctrl/alt/c is up
    keystroke.push_back(pta::CreateInput(VK_MENU, false));
    keystroke.push_back(pta::CreateInput(VK_CONTROL, false));
    keystroke.push_back(pta::CreateInput('C', false));

    keystroke.push_back(pta::CreateInput(VK_CONTROL, true));
    keystroke.push_back(pta::CreateInput('C', true));
    keystroke.push_back(pta::CreateInput('C', false));
    keystroke.push_back(pta::CreateInput(VK_CONTROL, false));

    SendInput(keystroke.size(), keystroke.data(), sizeof(keystroke[0]));

    m_blockHotkeys = false;
}

void PTA::handleClipboard()
{
    // delay this a bit
    QTimer::singleShot(50, this, &PTA::processClipboard);
}

void PTA::processClipboard()
{
    // Handle false cases:
    if (!m_pcTriggered)
    {
        // No search triggered
        return;
    }

    m_pcTriggered  = false; // handled
    m_blockHotkeys = false;

    if (!pta::IsPoEForeground())
    {
        return;
    }

    if (m_pctype < 0 || m_pctype >= PC_MAX)
    {
        qDebug() << "Bad price check type:" << m_pctype;
        return;
    }

    QString itemText = QGuiApplication::clipboard()->text();

    if (itemText.isEmpty())
    {
        // try winapi
        if (IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard(NULL))
        {
            HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
            if (hGlobal != NULL)
            {
                LPTSTR lpszData = (LPTSTR) GlobalLock(hGlobal);
                if (lpszData != NULL)
                {
                    itemText = QString::fromLocal8Bit((char*) lpszData);
                    GlobalUnlock(hGlobal);
                }
            }

            CloseClipboard();
        }

        if (itemText.isEmpty())
        {
            showToolTip("Failed to retrieve item text from clipboard. Please try again");
            qWarning() << "Failed to retrieve item text from clipboard.";
            return;
        }
    }

    showToolTip("Searching...");

    Item item;

    if (!m_api->parse(item, itemText))
    {
        showToolTip("Error parsing item text. Check log for more details.");
        qWarning() << "Error parsing item text" << itemText;
        return;
    }

    json data = json::object();

    data[p_item] = item;

    m_api->fillItemOptions(data);

    bool openUI   = true;
    bool forcetab = true;

    switch (m_pctype)
    {
        case PC_ADVANCED:
            data["tab"] = "mods";
            forcetab    = false;

        case PC_SIMPLE:
            // If item has filters and is not a map and is not unid'd
            if (item.contains(p_filters) && item[p_filters].size() && item[p_category] != "map" && !item.contains(p_unidentified))
            {
                // Load the price UI for advanced search

                // Qt fucks up whenever you mess with the clipboard within a QClipboard::dataChanged signal call,
                // and QWebEngineView does just that, so invoke it along the event queue instead

                openUI = false;
                QMetaObject::invokeMethod(
                    this, [=] { showPriceWidget(QString::fromStdString(data.dump())); }, Qt::QueuedConnection);
            }

            // Delay simple search attempt if we already have a window open
            QTimer::singleShot((openUI ? 0 : 500), [=] {
                if (!m_api->trySimplePriceCheck(data, openUI, forcetab))
                {
                    if (openUI)
                    {
                        showToolTip(tr("Price check is not available for this item type."));
                        qInfo() << "Price check is not available for this item type.";
                    }
                }
            });

            break;
        case WIKI_SEARCH:
            m_api->openWiki(item);
            break;
    }
}

void PTA::handleForegroundChange(bool isPoe)
{
    // Unset hotkeys if PoE not focused
    if (m_simpleKey)
    {
        m_simpleKey->setRegistered(isPoe);
    }

    if (m_advancedKey)
    {
        m_advancedKey->setRegistered(isPoe);
    }

    if (m_wikiKey)
    {
        m_wikiKey->setRegistered(isPoe);
    }
}

void PTA::processUpdates(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error())
    {
        qInfo() << reply->errorString();
        return;
    }

    QString       answer = reply->readAll();
    QJsonDocument doc    = QJsonDocument::fromJson(answer.toUtf8());

    if (doc.isNull())
    {
        qWarning() << "Error parsing Github API response";
        return;
    }

    auto info = doc.array();

    auto latest = info.at(0);
    auto latver = latest["name"].toString();

    // simple case compare should suffice
    if (latver > VER_STRING)
    {
        auto resp = QMessageBox::question(nullptr,
                                          tr("PTA Update"),
                                          tr("PTA version ") + latver + tr(" is available.\n\nWould you like to download it?"),
                                          QMessageBox::Yes | QMessageBox::No);

        if (resp == QMessageBox::Ok)
        {
            QDesktopServices::openUrl(QUrl(latest["html_url"].toString()));
        }
    }
    else
    {
        qInfo() << "No updates available. Already on the latest version.";
    }
}

PTA::InputHandler::InputHandler(PTA* parent) : m_parent(parent)
{
    using namespace std::placeholders;

    // Install hook
    pta::hook::InstallMouseHookCb(std::bind(&InputHandler::handleMouseEvent, this, _1, _2));
    pta::hook::InstallKeyboardHookCb(std::bind(&InputHandler::handleKeyboardEvent, this, _1, _2));
}

bool PTA::InputHandler::handleKeyboardEvent(WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*) lParam;

    if (kb->vkCode == VK_CONTROL || kb->vkCode == VK_LCONTROL || kb->vkCode == VK_RCONTROL)
    {
        m_ctrldown = (wParam == WM_KEYDOWN);
    }

    return false;
}

bool PTA::InputHandler::handleMouseEvent(WPARAM wParam, LPARAM lParam)
{
    if (wParam == WM_MOUSEWHEEL && m_parent && m_ctrldown && g_ctrlScrollEnabled && pta::IsPoEForeground())
    {
        MSLLHOOKSTRUCT* mhs = (MSLLHOOKSTRUCT*) lParam;

        auto zDelta = GET_WHEEL_DELTA_WPARAM(mhs->mouseData);

        QMetaObject::invokeMethod(m_parent, "handleScrollHotkey", Qt::AutoConnection, Q_ARG(short, zDelta));

        // consume the input
        return true;
    }

    return false;
}
