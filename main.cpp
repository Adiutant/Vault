#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QSettings>
#include <globals.h>
#include <QProcess>
#include "unistd.h"
#include <qtsingleapplication/src/QtSingleApplication>
#include "api/schemeeventfilter.h"

const QString protocol_layout(const QString & path) {
    return QString ("[Desktop Entry]"
           "Type=Application"
           "Name=vault protocol"
           "Exec=%1 %u"
           "StartupNotify=false"
           "MimeType=x-scheme-handler/vault;").arg(path);
}

const char* username() {
    return getlogin();
}

bool set_unix_uri(const QString & path) {
    const auto user_path = QString("/home/%1/.local/share/applications/vault-opener.desktop").arg(username());
    QFile f(user_path);
    if (!f.open(QIODevice::ReadWrite)) {
        qDebug() << f.errorString();
        return false;
    }
    f.write(protocol_layout(path).toUtf8());
    QProcess process;
    process.start("xdg-mime", QStringList() << "default " << user_path << " x-scheme-handler/vault");
    process.waitForFinished();;
    process.close();
    if (process.exitStatus() != 0) {
        return false;
    }
    return true;
}

bool set_windows_uri(const QString & path) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes",QSettings::NativeFormat);
    if /*(!settings.childGroups().contains("Vault"))*/(true){
        settings.beginGroup("Vault");
        settings.setValue("Default","URL:Vault Protocol");
        settings.setValue("DefaultIcon/Default","");
        settings.setValue("URL Protocol","");
        settings.setValue("shell/open/command/Default", QString("\"%1\"").arg(path) +"\"%1\"" );
        settings.endGroup();
    }
    return true;
}

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
#if defined(Q_OS_WIN)
    if (!set_windows_uri(path)) {
        qDebug() << "Error registering uri";
    }
#elif defined(Q_OS_LINUX)
    if (!set_unix_uri(path)) {
        qDebug() << "Error registering uri";
    }
#endif

    QCoreApplication::setApplicationName( QString("Vault") );

    MainWindow w;
    w.show();
    return a.exec();
}
