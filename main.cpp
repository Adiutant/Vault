#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <globals.h>
#include <qtsingleapplication/src/QtSingleApplication>
#include "api/schemeeventfilter.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);

    if (a.isRunning()) {
        if (a.arguments().size() > 1) {
            for(int i = 1; i < a.arguments().size(); i++) {
                a.sendMessage(a.arguments().at(i), 500);
            }
        }
        return 0;
    }
    auto filter = std::make_shared<SchemeEventFilter>();
    QObject::connect(&a, &QtSingleApplication::messageReceived, filter.get(), &SchemeEventFilter::handleURL);
    VaultGlobal::schemeHandler = filter;
    QString path = QDir::toNativeSeparators(qApp->applicationFilePath());
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes",QSettings::NativeFormat);
    if /*(!settings.childGroups().contains("Vault"))*/(true){
        settings.beginGroup("Vault");
        settings.setValue("Default","URL:Vault Protocol");
        settings.setValue("DefaultIcon/Default","");
        settings.setValue("URL Protocol","");
        settings.setValue("shell/open/command/Default", QString("\"%1\"").arg(path) +"\"%1\"" );
        settings.endGroup();
    }
    QCoreApplication::setApplicationName( QString("Vault") );

    MainWindow w;
    w.show();
    return a.exec();
}
