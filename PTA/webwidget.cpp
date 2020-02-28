#include "webwidget.h"
#include "itemapi.h"
#include "pta_types.h"

#include <QVBoxLayout>
#include <QWebChannel>
#include <QtWebEngineWidgets/QWebEngineCertificateError>
#include <QtWebEngineWidgets/QWebEngineScriptCollection>
#include <QtWebEngineWidgets/QWebEngineSettings>

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

WebWidget::WebWidget(ItemAPI* api, const QString& data, QWidget* parent) : FramelessWindow(parent)
{
    QIcon icon(":/Resources/logo.svg");
    setWindowIcon(icon);

    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings;

    webview = new PWebView(this);
    webview->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
    webview->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, false);

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

    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("api"), api);
    page->setWebChannel(channel);

    page->load(startFile);

    webview->setPage(page);

    connect(page, &QWebEnginePage::windowCloseRequested, this, &QWidget::close);

    // resize
    int default_width  = settings.value(PTA_CONFIG_TEMPLATE_WIDTH, PTA_CONFIG_DEFAULT_TEMPLATE_WIDTH).toInt();
    int default_height = settings.value(PTA_CONFIG_TEMPLATE_HEIGHT, PTA_CONFIG_DEFAULT_TEMPLATE_HEIGHT).toInt();

    QSize defaultsize = {default_width, default_height};

    webview->resize(defaultsize);
    resize(defaultsize);

    // No context menu
    setContextMenuPolicy(Qt::PreventContextMenu);

    // Inject global script
    QString dataScript = generateDataScript(data);
    script.setName("Data");
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    script.setWorldId(0);
    script.setSourceCode(dataScript);
    webview->page()->scripts().insert(script);

    setWindowTitle("PTA Price Check");

    setContent(webview);

    // Restore settings
    restoreGeometry(settings.value(getSettingKey("geometry")).toByteArray());
}

WebWidget::~WebWidget()
{
    saveSettings();
}

QString WebWidget::generateDataScript(const QString& data)
{
    QString pscript = QString("var item = %1;").arg(data);
    return pscript;
}

void WebWidget::saveSettings()
{
    QSettings settings;
    settings.setValue(getSettingKey("geometry"), saveGeometry());
}

QString WebWidget::getSettingKey(QString key)
{
    return "widget/" + key;
}
