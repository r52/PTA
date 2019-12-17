#pragma once

#include "ui_pta.h"

#include <memory>

#include <QAbstractNativeEventFilter>
#include <QHotkey>
#include <QSystemTrayIcon>
#include <QtWidgets/QMainWindow>

QT_FORWARD_DECLARE_CLASS(LogWindow)
QT_FORWARD_DECLARE_CLASS(ItemAPI)

struct PItem;

class PTA : public QMainWindow
{
    Q_OBJECT

    class InputHandler : public QAbstractNativeEventFilter
    {
    public:
        InputHandler(QObject* parent = nullptr);

        virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

    private:
        QObject* m_parent;
    };

public:
    explicit PTA(LogWindow* log, QWidget* parent = Q_NULLPTR);
    ~PTA();

public slots:
    void showToolTip(QString message);
    void showPriceResults(std::shared_ptr<PItem> item, QString results);

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    void createTrayIcon();
    void createActions();

    void setupFunctionality();

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void priceCheckActivated();
    void advancedPriceCheckActivated();
    void handleScrollHotkey(quint16 data);

public:
    // Input Handler
    InputHandler m_inputhandler;

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

    bool m_blockHotkeys;

    Q_DISABLE_COPY(PTA);
};
