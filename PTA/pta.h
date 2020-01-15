#pragma once

#include "ui_pta.h"

#include "macrohandler.h"

#include <memory>

#include <QAbstractNativeEventFilter>
#include <QHotkey>
#include <QSystemTrayIcon>
#include <QtWidgets/QMainWindow>

QT_FORWARD_DECLARE_CLASS(LogWindow)
QT_FORWARD_DECLARE_CLASS(ItemAPI)

struct PItem;

enum price_check_flag : uint32_t
{
    PC_SIMPLE = 0,
    PC_ADVANCED,
    PC_MAX
};

class PTA : public QMainWindow
{
    Q_OBJECT

    class InputHandler
    {
    public:
        InputHandler(PTA* parent = nullptr);

    private:
        bool handleKeyboardEvent(WPARAM wParam, LPARAM lParam);
        bool handleMouseEvent(WPARAM wParam, LPARAM lParam);

    private:
        PTA* m_parent;
        bool m_ctrldown = false;
    };

public:
    explicit PTA(LogWindow* log, QWidget* parent = Q_NULLPTR);
    ~PTA();

signals:
    void foregroundWindowChanged(bool isPoe);

public slots:
    void showToolTip(QString message);
    void showPriceResults(std::shared_ptr<PItem> item, QString results);

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    void createTrayIcon();
    void createActions();

    void setupFunctionality();

    void foregroundEventCb(bool isPoe);

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void handleScrollHotkey(short data);
    void handlePriceCheckHotkey(uint32_t flag);
    void handleClipboard();
    void processClipboard();

private:
    Ui::PTAClass ui;

    // Log Window
    LogWindow* m_logWindow;

    // Tray
    QSystemTrayIcon* m_trayIcon;
    QMenu*           m_trayIconMenu;

    // Actions/menus
    QAction* m_settingsAction;
    QAction* m_logAction;
    QAction* m_suspendAction;
    QAction* m_aboutAction;
    QAction* m_aboutQtAction;
    QAction* m_quitAction;

    // API
    ItemAPI* m_api;

    // Hotkeys
    std::unique_ptr<QHotkey> m_simpleKey;
    std::unique_ptr<QHotkey> m_advancedKey;

    // Macros
    MacroHandler m_macrohandler;

    // Input Handler
    InputHandler m_inputhandler;

    bool     m_blockHotkeys = false;
    bool     m_pcTriggered  = false;
    uint32_t m_pctype;

    Q_DISABLE_COPY(PTA);
};
