#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>
#include <QDebug>
#ifdef Q_OS_WIN32
    #define WINVER 0x0500
    #include <Windows.h>
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



static void sendKeyEventToSystem(Qt::Key qtKey) { //функция эмуляции нажатия клавиши. qtKey - тип клавиши
#ifdef Q_OS_WIN32
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
#endif
}
#endif // GLOBALS_H
