#pragma once
#include "pitem.h"

#include <framelesswindow.h>

#include <QWidget>
#include <QtGui>
#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QtWebEngineWidgets/QWebEngineScript>
#include <QtWebEngineWidgets/QWebEngineView>

QT_FORWARD_DECLARE_CLASS(ItemAPI);

class PWebView : public QWebEngineView
{
public:
    PWebView(QWidget* parent = nullptr) : QWebEngineView{parent} { setContextMenuPolicy(Qt::NoContextMenu); }
};

class PWebPage : public QWebEnginePage
{
public:
    PWebPage(QObject* parent = nullptr) : QWebEnginePage{parent} {}
    PWebPage(QWebEngineProfile* profile, QObject* parent = nullptr) : QWebEnginePage{profile, parent} {}

protected:
    virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID) override;
};

class WebWidget : public FramelessWindow
{
    Q_OBJECT

public:
    explicit WebWidget(ItemAPI* api, const QString& data, QWidget* parent = nullptr);
    ~WebWidget();

    void saveSettings();

    static QString generateDataScript(const QString& data);

private:
    QString getSettingKey(QString key);

    // Web engine widget
    PWebView* webview;

    // Page script
    QWebEngineScript script;

    Q_DISABLE_COPY(WebWidget);
};
