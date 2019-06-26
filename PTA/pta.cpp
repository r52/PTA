#include "pta.h"

#include "itemparser.h"
#include "logwindow.h"
#include "papi.h"
#include "webwidget.h"

#include <QApplication>
#include <QClipboard>
#include <QHotkey>
#include <QMenu>
#include <QMessageBox>
#include <QTextEdit>
#include <QToolTip>
#include <QVBoxLayout>

#include <Windows.h>

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

    QString itemjson = m_parser->toJson(item.get());

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
    m_api = new PAPI(this);

    connect(m_api, &PAPI::humour, this, &PTA::showToolTip);
    connect(m_api, &PAPI::priceCheckFinished, this, &PTA::showPriceResults);

    // Initialize parser
    m_parser = new ItemParser(this);

    // Price Check
    auto hotkey = new QHotkey(QKeySequence("ctrl+D"), true, this);
    qDebug() << "Price Check Hotkey Registered:" << hotkey->isRegistered();

    connect(hotkey, &QHotkey::activated, this, &PTA::priceCheckActivated);
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
    QMetaObject::Connection* clip = new QMetaObject::Connection;

    *clip = connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, [=]() {
        disconnect(*clip);
        delete clip;

        QString itemText = QGuiApplication::clipboard()->text();

        showToolTip("Searching...");

        std::shared_ptr<PItem> item(m_parser->parse(itemText));
        m_api->simplePriceCheck(item);
    });

    // Send ctrl-c

    INPUT ip;
    ip.type           = INPUT_KEYBOARD;
    ip.ki.wScan       = 0;
    ip.ki.time        = 0;
    ip.ki.dwExtraInfo = 0;

    ip.ki.wVk     = VK_CONTROL;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk     = 'C';
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk     = 'C';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk     = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    m_blockHotkeys = false;
}
