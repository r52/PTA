#pragma once

#include <QWidget>
#include <QtGui>
#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QtWebEngineWidgets/QWebEngineScript>
#include <QtWebEngineWidgets/QWebEngineView>

QT_FORWARD_DECLARE_CLASS(QMenu);

// From https://stackoverflow.com/questions/19362455/dark-transparent-layer-over-a-qmainwindow-in-qt
class OverlayWidget : public QWidget
{
    void newParent()
    {
        if (!parent())
            return;
        parent()->installEventFilter(this);
        raise();
    }

public:
    explicit OverlayWidget(QWidget* parent = {}) : QWidget{parent}
    {
        setAttribute(Qt::WA_NoSystemBackground);
        newParent();
    }

protected:
    //! Catches resize and child events from the parent widget
    bool eventFilter(QObject* obj, QEvent* ev) override
    {
        if (obj == parent())
        {
            if (ev->type() == QEvent::Resize)
                resize(static_cast<QResizeEvent*>(ev)->size());
            else if (ev->type() == QEvent::ChildAdded)
                raise();
        }
        return QWidget::eventFilter(obj, ev);
    }
    //! Tracks parent widget changes
    bool event(QEvent* ev) override
    {
        if (ev->type() == QEvent::ParentAboutToChange)
        {
            if (parent())
                parent()->removeEventFilter(this);
        }
        else if (ev->type() == QEvent::ParentChange)
            newParent();
        return QWidget::event(ev);
    }
};

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
    virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID);
};

class WebWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WebWidget(QString item, QString results, QWidget* parent = nullptr);
    ~WebWidget();

    static QString generateDataScript(QString item, QString results);

    void saveSettings();

protected:
    // Overrides
    virtual void mousePressEvent(QMouseEvent* evt) override;
    virtual void mouseMoveEvent(QMouseEvent* evt) override;

private:
    QString getSettingKey(QString key);

    static QString DataScript;

    // Web engine widget
    PWebView* webview;

    // Widget overlay
    OverlayWidget* overlay;

    // Drag and drop pos
    QPoint dragPosition;

    // Page script
    QWebEngineScript script;

    Q_DISABLE_COPY(WebWidget);
};
