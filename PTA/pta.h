#pragma once

#include "ui_pta.h"

#include <QSystemTrayIcon>
#include <QtWidgets/QMainWindow>

QT_FORWARD_DECLARE_CLASS(LogWindow)
QT_FORWARD_DECLARE_CLASS(ItemParser)
QT_FORWARD_DECLARE_CLASS(PAPI)

class PTA : public QMainWindow
{
    Q_OBJECT

public:
    explicit PTA(LogWindow* log, QWidget* parent = Q_NULLPTR);

private:
    void createTrayIcon();
    void createActions();

    void setupFunctionality();

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void priceCheckActivated();

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
    QAction* m_aboutAction;
    QAction* m_aboutQtAction;
    QAction* m_quitAction;

    //
    PAPI*       m_api;
    ItemParser* m_parser;

    bool m_blockHotkeys;

    Q_DISABLE_COPY(PTA);
};
