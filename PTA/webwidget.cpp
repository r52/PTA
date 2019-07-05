#include "webwidget.h"
#include "pitem.h"
#include "pta_types.h"

#include <QtWebEngineWidgets/QWebEngineCertificateError>
#include <QtWebEngineWidgets/QWebEngineScriptCollection>
#include <QtWebEngineWidgets/QWebEngineSettings>

QString WebWidget::DataScript;

void PWebPage::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID)
{
    QString msg = "CONSOLE: " + message + " (" + sourceID + ":" + QString::number(lineNumber) + ")";

    switch (level)
    {
        case InfoMessageLevel:
            qInfo().noquote() << msg;
            break;
        case WarningMessageLevel:
            qWarning().noquote() << msg;
            break;
        case ErrorMessageLevel:
            qCritical().noquote() << msg;
            break;
    }
}

WebWidget::WebWidget(QString item, QString results, QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings;

    // No frame/border, no taskbar button
    Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint;

    webview = new PWebView(this);

    QUrl startFile;

    QString tmplpath = settings.value(PTA_CONFIG_PRICE_TEMPLATE, PTA_CONFIG_DEFAULT_PRICE_TEMPLATE).toString();

    QFileInfo tmplFile(tmplpath);

    if (!tmplFile.exists())
    {
        qWarning() << "Template file does not exist.";
    }

    QString startFilePath = tmplFile.absoluteFilePath();
    startFile             = QUrl::fromLocalFile(startFilePath);

    PWebPage* page = new PWebPage(this);
    page->load(startFile);
    webview->setPage(page);

    webview->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);

    // Overlay for catching drag and drop events
    overlay = new OverlayWidget(this);

    // Restore settings
    restoreGeometry(settings.value(getSettingKey("geometry")).toByteArray());

    // resize
    int default_width  = settings.value(PTA_CONFIG_TEMPLATE_WIDTH, PTA_CONFIG_DEFAULT_TEMPLATE_WIDTH).toInt();
    int default_height = settings.value(PTA_CONFIG_TEMPLATE_HEIGHT, PTA_CONFIG_DEFAULT_TEMPLATE_HEIGHT).toInt();

    QSize defaultsize = {default_width, default_height};

    webview->resize(defaultsize);
    resize(defaultsize);

    overlay->setVisible(true);

    // No context menu
    setContextMenuPolicy(Qt::PreventContextMenu);

    // Set flags
    setWindowFlags(flags);

    // Inject global script
    QString dataScript = generateDataScript(item, results);

    script.setName("Data");
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    script.setWorldId(0);
    script.setSourceCode(dataScript);

    webview->page()->scripts().insert(script);

    setWindowTitle("Price Check");
}

WebWidget::~WebWidget()
{
    saveSettings();
}

QString WebWidget::generateDataScript(QString item, QString results)
{
    if (DataScript.isEmpty())
    {
        QFile file(":/Resources/data.js");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw std::runtime_error("data script load failure");
        }

        QTextStream in(&file);
        DataScript = in.readAll();
    }

    QString pscript = DataScript.arg(item).arg(results);

    return pscript;
}

void WebWidget::saveSettings()
{
    QSettings settings;
    settings.setValue(getSettingKey("geometry"), saveGeometry());
}

void WebWidget::mousePressEvent(QMouseEvent* evt)
{
    if (evt->button() == Qt::LeftButton)
    {
        dragPosition = evt->globalPos() - frameGeometry().topLeft();
        evt->accept();
    }

    if (evt->button() == Qt::RightButton)
    {
        evt->accept();
        close();
    }
}

void WebWidget::mouseMoveEvent(QMouseEvent* evt)
{
    if (evt->buttons() & Qt::LeftButton)
    {
        move(evt->globalPos() - dragPosition);
        evt->accept();
    }
}

QString WebWidget::getSettingKey(QString key)
{
    return "widget/" + key;
}
