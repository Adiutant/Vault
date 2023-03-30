#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>
#include <QDebug>
#include <QSettings>
#include "api/schemeeventfilter.h"
#ifdef Q_OS_WIN32
    #define WINVER 0x0500
    #include <Windows.h>
    static void sendKeyEventToSystem(Qt::Key qtKey) {
        Q_UNUSED(qtKey)//функция эмуляции нажатия клавиши. qtKey - тип клавиши

        INPUT ip; //устройство ввода
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0;
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        //посылаем событие нажатия клавиши
        ip.ki.dwFlags = 0;
        SendInput(1, &ip, sizeof(INPUT));
        //а затем отжатия клавиши
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
    }
#endif
#define TIMEOUT 3600000
const static QString FILENAME ="cr.hs";
struct PasswordData{
        QString key;
    QString password;

};
struct Key{
    bool upper;
    Qt::Key key;
};

const QString YADISK_SET  = "YandexDiskSet";
const QString YADISK_AUTH  = "YandexDiskAuth";

const QString YADISK_AUTH_IV  = "YandexDiskAuthIv";


class VaultSettings : public QSettings {
    Q_OBJECT
public:
    VaultSettings(QObject *parent)
        : QSettings("settings.ini", QSettings::IniFormat, parent)
    {

    }

    void updateValue(const QString &key, const QVariant &val)
    {
        if (value(key) != val) {
            setValue(key, val);
            emit valueChanged(key);
        } else {
            setValue(key, val);
        }
    }
signals:
    void valueChanged(const QString &key); ///< Настройка по ключу изменилась

};
class VaultGlobal{
public:
    static VaultSettings* SETTINGS;

        static SchemeEventFilter* schemeHandler;
};



#endif // GLOBALS_H
