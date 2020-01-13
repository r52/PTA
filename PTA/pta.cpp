#include "pta.h"

#include "configdialog.h"
#include "itemapi.h"
#include "logwindow.h"
#include "pta_types.h"
#include "version.h"
#include "webwidget.h"

#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QToolTip>
#include <QVBoxLayout>

#include <Windows.h>

namespace
{
    bool g_ctrlScrollEnabled = false;
}

INPUT createInput(WORD vk, bool isDown)
{
    INPUT input          = {};
    input.type           = INPUT_KEYBOARD;
    input.ki.wVk         = vk;
    input.ki.wScan       = 0;
    input.ki.dwFlags     = (isDown ? 0 : KEYEVENTF_KEYUP);
    input.ki.time        = 0;
    input.ki.dwExtraInfo = 0;
    return input;
}

PTA::PTA(LogWindow* log, QWidget* parent) : QMainWindow(parent), m_logWindow(log), m_inputhandler(this)
{
    if (nullptr == m_logWindow)
    {
        throw std::invalid_argument("Invalid LogWindow");
    }

    m_logWindow->setParent(this);

    ui.setupUi(this);

    // Try to initialize API
    try
    {
        m_api = new ItemAPI(this);
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

    // Setup functionality
    setupFunctionality();
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
}

bool PTA::isPoEForeground()
{
    static const std::wstring s_poeCls = L"POEWindowClass";

    HWND hwnd = GetForegroundWindow();

    if (nullptr == hwnd)
    {
        return false;
    }

    wchar_t cls[512];
    GetClassName(hwnd, cls, std::size(cls));

    if (s_poeCls != cls)
    {
        qDebug() << "Active window not PoE";
        return false;
    }

    return true;
}

void PTA::showToolTip(QString message)
{
    QToolTip::showText(QCursor::pos() + QPoint(5, 20), message);
}

void PTA::showPriceResults(std::shared_ptr<PItem> item, QString results)
{
#ifndef NDEBUG
    qDebug() << "Prices copied to clipboard";
    QGuiApplication::clipboard()->setText(results);
#endif

    showToolTip(QString());

    QString itemjson = m_api->toJson(item.get());

    auto pricedlg = new WebWidget(itemjson, results);
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
    connect(m_settingsAction, &QAction::triggered, [&] {
        json results;

        ConfigDialog dlg(results, m_api);
        int          ret = dlg.exec();

        if (!ret)
        {
            // User close
            return;
        }

        QSettings settings;

        for (auto& [k, v] : results.items())
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

                    connect(m_simpleKey.get(), &QHotkey::activated, [=]() { handlePriceCheckHotkey(PC_SIMPLE); });
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

                    connect(m_advancedKey.get(), &QHotkey::activated, [=]() { handlePriceCheckHotkey(PC_ADVANCED); });
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

            if (k == PTA_CONFIG_CTRL_SCROLL_HOTKEY_ENABLED)
            {
                g_ctrlScrollEnabled = v.get<bool>();
            }
        }
    });

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
    connect(m_api, &ItemAPI::humour, this, &PTA::showToolTip);
    connect(m_api, &ItemAPI::priceCheckFinished, this, &PTA::showPriceResults);

    // Hotkeys
    QSettings settings;

    bool simpleEnabled = settings.value(PTA_CONFIG_SIMPLE_CHECK_HOTKEY_ENABLED, true).toBool();

    if (simpleEnabled)
    {
        QString seq = settings.value(PTA_CONFIG_SIMPLE_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_SIMPLE_CHECK_HOTKEY).toString();

        m_simpleKey.reset(new QHotkey(QKeySequence(seq), true));
        qDebug() << "Price Check Hotkey Registered:" << m_simpleKey->isRegistered();

        connect(m_simpleKey.get(), &QHotkey::activated, [=]() { handlePriceCheckHotkey(PC_SIMPLE); });
    }

    bool advEnabled = settings.value(PTA_CONFIG_ADV_CHECK_HOTKEY_ENABLED, true).toBool();

    if (advEnabled)
    {
        QString seq = settings.value(PTA_CONFIG_ADV_CHECK_HOTKEY, PTA_CONFIG_DEFAULT_ADV_CHECK_HOTKEY).toString();

        m_advancedKey.reset(new QHotkey(QKeySequence(seq), true));
        qDebug() << "Advanced Price Check Hotkey Registered:" << m_advancedKey->isRegistered();

        connect(m_advancedKey.get(), &QHotkey::activated, [=]() { handlePriceCheckHotkey(PC_ADVANCED); });
    }

    // ctrl + scroll
    g_ctrlScrollEnabled = settings.value(PTA_CONFIG_CTRL_SCROLL_HOTKEY_ENABLED, true).toBool();

    // Clipboard
    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &PTA::handleClipboard);

    // Setup Raw Input
    auto hwnd = (HWND) winId();

    RAWINPUTDEVICE rid[2];

    // Keyboard
    rid[0].usUsagePage = 1;
    rid[0].usUsage     = 6;
    rid[0].dwFlags     = RIDEV_INPUTSINK;
    rid[0].hwndTarget  = hwnd;

    // Mouse
    rid[1].usUsagePage = 1;
    rid[1].usUsage     = 2;
    rid[1].dwFlags     = RIDEV_INPUTSINK;
    rid[1].hwndTarget  = hwnd;

    if (RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
    {
        qWarning() << "Error registering Raw Input devices. Mouse macros disabled.";
        qWarning() << "Error code:" << QString::number(GetLastError());
    }
}

void PTA::handleScrollHotkey(quint16 data)
{
    qint16 dirdat = (qint16) data;

    WORD key = (dirdat > 0 ? VK_LEFT : VK_RIGHT);

    // Send input
    std::vector<INPUT> keystroke;

    keystroke.push_back(createInput(key, true));
    keystroke.push_back(createInput(key, false));

    SendInput(keystroke.size(), keystroke.data(), sizeof(keystroke[0]));
}

void PTA::handlePriceCheckHotkey(uint32_t flag)
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
    if (!isPoEForeground())
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
    keystroke.push_back(createInput(VK_MENU, false));
    keystroke.push_back(createInput(VK_CONTROL, false));
    keystroke.push_back(createInput('C', false));

    keystroke.push_back(createInput(VK_CONTROL, true));
    keystroke.push_back(createInput('C', true));
    keystroke.push_back(createInput('C', false));
    keystroke.push_back(createInput(VK_CONTROL, false));

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

    m_pcTriggered = false; // handled

    if (!isPoEForeground())
    {
        m_blockHotkeys = false;
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
                    itemText.fromLocal8Bit((const char*) lpszData);
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

    std::shared_ptr<PItem> item(m_api->parse(itemText));

    if (!item)
    {
        showToolTip("Error parsing item text. Check log for more details.");
        qWarning() << "Error parsing item text" << itemText;
        return;
    }

    switch (m_pctype)
    {
        case PC_SIMPLE:
            m_api->simplePriceCheck(item);
            break;
        case PC_ADVANCED:
            m_api->advancedPriceCheck(item);
            break;
    }
}

PTA::InputHandler::InputHandler(PTA* parent) : m_parent(parent) {}

bool PTA::InputHandler::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
    if (eventType == "windows_generic_MSG")
    {
        MSG* msg = reinterpret_cast<MSG*>(message);

        if (msg->message == WM_INPUT)
        {
            UINT dwSize;
            GetRawInputData((HRAWINPUT) msg->lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

            LPBYTE lpb = new BYTE[dwSize];
            if (lpb == nullptr)
            {
                return false;
            }
            if (GetRawInputData((HRAWINPUT) msg->lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
            {
                qDebug() << "GetRawInputData did not return correct size";
            }
            else
            {
                RAWINPUT* raw = (RAWINPUT*) lpb;
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    if (raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL)
                    {
                        if (m_parent && m_ctrldown && g_ctrlScrollEnabled)
                        {
                            if (!m_parent->isPoEForeground())
                            {
                                return false;
                            }

                            QMetaObject::invokeMethod(m_parent, "handleScrollHotkey", Qt::AutoConnection, Q_ARG(quint16, raw->data.mouse.usButtonData));
                        }
                    }
                }

                if (raw->header.dwType == RIM_TYPEKEYBOARD)
                {
                    USHORT keyCode = raw->data.keyboard.VKey;
                    bool   keyUp   = raw->data.keyboard.Flags & RI_KEY_BREAK;

                    if (keyCode == VK_CONTROL)
                    {
                        m_ctrldown = !keyUp;
                    }
                }
            }
        }
    }

    return false;
}
