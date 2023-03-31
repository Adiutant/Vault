#ifndef YANDEXAPI_H
#define YANDEXAPI_H
#include <globals.h>
#include <QMutex>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QNetworkReply>
#include <QJsonDocument>

#include <QJsonObject>
#include <QUrlQuery>
#include "schemeeventfilter.h"

class YandexApi : public QThread
{
    Q_OBJECT
     const QString  OUATHGET_METHOD = "https://oauth.yandex.ru/authorize?response_type=code&client_id=d393faa55762423388db7d8946a1d719";
     const QString  OUATHFINISH_METHOD = "https://oauth.yandex.ru/token";
     const QString DISK_METADATA = "https://cloud-api.yandex.net/v1/disk?path=app:/";
     const QString DISK_DOWNLOAD = "https://cloud-api.yandex.net/v1/disk/resources/download?path=app:/cr.hs";
     const QString DISK_UPLOAD = "https://cloud-api.yandex.net/v1/disk/resources/upload?path=app:/cr.hs&overwrite=true";
    const QString  CONNECT_METHOD = "";
public:
    YandexApi();

    void createConnection();
    void syncData(const QString &token);
    void uploadData(const QByteArray& data,const QString &token);

private:
    void getOAuthToken();
     void getDataFromDisk(const QString &token);
    void setDataToDisk(const QByteArray& data,const QString &token);
    QMutex *syncMutex;
    QNetworkAccessManager* apiManager;
private slots:
    void replyFinished(QNetworkReply *reply);
    void codeRecieved(const QString&code);
signals:
    void tokenGranted(const QString& token );
    void dataGranted(const QByteArray& data);
    void dataUploaded();
    void emptyDiskData();
};

#endif // YANDEXAPI_H
