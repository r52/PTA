#include "pta.h"

#include "logwindow.h"
#include "runguard.h"

#include <QSettings>
#include <QtWidgets/QApplication>

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

    PTA w(log);
    w.hide();

    return a.exec();
}
