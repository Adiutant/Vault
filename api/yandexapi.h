#ifndef YANDEXAPI_H
#define YANDEXAPI_H
#include <globals.h>
#include <QMutex>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QNetworkReply>
#include "schemeeventfilter.h"

class YandexApi : public QThread
{
     const QString  OUATHGET_METHOD = "https://oauth.yandex.ru/authorize?response_type=code&client_id=597189b7fa1b4c04a024e4ec2bee105d";
    const QString  CONNECT_METHOD = "";
public:
    YandexApi();

    void createConnection();

private:
    void getOAuthToken();
    QMutex *syncMutex;
    QNetworkAccessManager* apiManager;
private slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // YANDEXAPI_H
