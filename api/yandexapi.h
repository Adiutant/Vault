#ifndef YANDEXAPI_H
#define YANDEXAPI_H
#include <globals.h>
#include <QMutex>
#include <QThread>
#include <QNetworkAccessManager>

class YandexApi : public QThread
{
    const QString  CONNECT_METHOD = "";
public:
    YandexApi();

    void createConnection();

private:
    QMutex *syncMutex;
    QNetworkAccessManager* apiManager;
};

#endif // YANDEXAPI_H
