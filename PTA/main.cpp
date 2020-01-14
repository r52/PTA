#include "pta.h"

#include "logwindow.h"
#include "runguard.h"

#include <QSettings>
#include <QSplashScreen>
#include <QtWidgets/QApplication>

#include <Windows.h>

int main(int argc, char* argv[])
{
    RunGuard guard("pta_app_key");
    if (!guard.tryToRun())
        return 0;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("PTA");
    QCoreApplication::setOrganizationName("PTA");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    LogWindow* log = new LogWindow();

    QPixmap       pixmap(":/Resources/splash.png");
    QSplashScreen splash(pixmap);
    auto          align = Qt::AlignHCenter | Qt::AlignBottom;
    auto          color = Qt::white;
    splash.show();

    splash.showMessage("Downloading API data...", align, color);
    a.processEvents();

    PTA w(log);

    w.hide();

    splash.finish(&w);

    return a.exec();
}
