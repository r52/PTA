#include "pta.h"

#include "itemapi.h"
#include "logwindow.h"
#include "webwidget.h"

#include <once/once.h>

#include <QApplication>
#include <QClipboard>
#include <QHotkey>
#include <QMenu>
#include <QMessageBox>
#include <QTextEdit>
#include <QToolTip>
#include <QVBoxLayout>

#include <Windows.h>

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

PTA::PTA(LogWindow* log, QWidget* parent) : QMainWindow(parent), m_logWindow(log), m_blockHotkeys(false)
{
    if (nullptr == m_logWindow)
    {
        throw std::invalid_argument("Invalid LogWindow");
    }

    m_logWindow->setParent(this);

    ui.setupUi(this);

    // Setup system tray
    createActions();
    createTrayIcon();

    // Setup icon
    QIcon icon(":/PTA.ico");
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
        // TODO Settings dialog
    });

    m_logAction = new QAction(tr("L&og"), this);
    connect(m_logAction, &QAction::triggered, this, &QWidget::showNormal);

    m_suspendAction = new QAction(tr("Sus&pend Hotkeys"), this);
    m_suspendAction->setCheckable(true);
    m_suspendAction->setChecked(m_blockHotkeys);
    connect(m_suspendAction, &QAction::triggered, [&](bool checked) { m_blockHotkeys = checked; });

    m_aboutAction = new QAction(tr("&About PTA"), this);

    connect(m_aboutAction, &QAction::triggered, [&] {
        static QString aboutMsg = "PTA v1.0"
                                  "<br/>"
#ifndef NDEBUG
                                  "DEBUG BUILD<br/>"
#endif
                                  "<br/>"
                                  "Compiled on: " __DATE__ "<br/>"
                                  "Qt version: " QT_VERSION_STR "<br/>"
                                  "<br/>"
                                  "Licensed under GPLv3<br/>"
                                  "Source code available at <a href='https://github.com/r52/PTA'>GitHub</a>";

        QMessageBox::about(this, tr("About Quasar"), aboutMsg);
    });

    m_aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(m_aboutQtAction, &QAction::triggered, [&] { QMessageBox::aboutQt(this, "About Qt"); });

    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void PTA::setupFunctionality()
{
    // Initialize API
    m_api = new ItemAPI(this);

    connect(m_api, &ItemAPI::humour, this, &PTA::showToolTip);
    connect(m_api, &ItemAPI::priceCheckFinished, this, &PTA::showPriceResults);

    // Price Check
    auto hotkey = new QHotkey(QKeySequence("Ctrl+D"), true, this);
    qDebug() << "Price Check Hotkey Registered:" << hotkey->isRegistered();

    connect(hotkey, &QHotkey::activated, this, &PTA::priceCheckActivated);

    auto advhotkey = new QHotkey(QKeySequence("Ctrl+Alt+D"), true, this);
    qDebug() << "Advanced Price Check Hotkey Registered:" << advhotkey->isRegistered();

    connect(advhotkey, &QHotkey::activated, this, &PTA::advancedPriceCheckActivated);
}

void PTA::priceCheckActivated()
{
    if (m_blockHotkeys)
    {
        // Currently blocked
        return;
    }

    m_blockHotkeys = true;

    // Check for PoE window
    HWND hwnd = GetForegroundWindow();

    if (nullptr == hwnd)
    {
        m_blockHotkeys = false;
        qDebug() << "Null active window.";
        return;
    }

    std::wstring t;
    t.reserve(64);
    GetClassName(hwnd, const_cast<WCHAR*>(t.c_str()), t.capacity());

    QString wcls = QString::fromWCharArray(t.c_str());

    if ("POEWindowClass" != wcls)
    {
        m_blockHotkeys = false;
        qDebug() << "Active window not PoE";
        return;
    }

    // Set up clipboard signal
    Once::connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, [=]() {
        QString itemText = QGuiApplication::clipboard()->text();

        showToolTip("Searching...");

        std::shared_ptr<PItem> item(m_api->parse(itemText));
        m_api->simplePriceCheck(item);
    });

    // Send ctrl-c
    std::vector<INPUT> keystroke;

    keystroke.push_back(createInput(VK_CONTROL, true));
    keystroke.push_back(createInput('C', true));
    keystroke.push_back(createInput('C', false));
    keystroke.push_back(createInput(VK_CONTROL, false));

    SendInput(keystroke.size(), keystroke.data(), sizeof(keystroke[0]));

    m_blockHotkeys = false;
}

void PTA::advancedPriceCheckActivated()
{
    if (m_blockHotkeys)
    {
        // Currently blocked
        return;
    }

    m_blockHotkeys = true;

    // Check for PoE window
    HWND hwnd = GetForegroundWindow();

    if (nullptr == hwnd)
    {
        m_blockHotkeys = false;
        qDebug() << "Null active window.";
        return;
    }

    std::wstring t;
    t.reserve(64);
    GetClassName(hwnd, const_cast<WCHAR*>(t.c_str()), t.capacity());

    QString wcls = QString::fromWCharArray(t.c_str());

    if ("POEWindowClass" != wcls)
    {
        m_blockHotkeys = false;
        qDebug() << "Active window not PoE";
        return;
    }

    // Set up clipboard signal
    Once::connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, [=]() {
        QString itemText = QGuiApplication::clipboard()->text();

        showToolTip("Searching...");

        std::shared_ptr<PItem> item(m_api->parse(itemText));
        m_api->advancedPriceCheck(item);
    });

    // Send ctrl-c
    std::vector<INPUT> keystroke;

    // ensure alt is up
    keystroke.push_back(createInput(VK_MENU, false));

    keystroke.push_back(createInput(VK_CONTROL, true));
    keystroke.push_back(createInput('C', true));
    keystroke.push_back(createInput('C', false));
    keystroke.push_back(createInput(VK_CONTROL, false));

    SendInput(keystroke.size(), keystroke.data(), sizeof(keystroke[0]));

    m_blockHotkeys = false;
}
