#include "yandexapi.h"

YandexApi::YandexApi()
{

    syncMutex = new QMutex();
    apiManager = new QNetworkAccessManager(this);
    connect(apiManager, &QNetworkAccessManager::finished,
            this, &YandexApi::replyFinished,Qt::DirectConnection);
    connect (VaultGlobal::schemeHandler, &SchemeEventFilter::incomingURL, this, &YandexApi::codeRecieved);
    apiManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
}

void YandexApi::createConnection()
{
    if(VaultGlobal::SETTINGS->value(YADISK_AUTH).toString().isEmpty()){
       QDesktopServices::openUrl(OUATHGET_METHOD);

    }
}

void YandexApi::syncData(const QString &token)
{
    auto req = QNetworkRequest();
    req.setUrl(QString(DISK_METADATA));

    req.setRawHeader("Authorization",QString( "OAuth " ).append(token ).toUtf8());
    auto reply = apiManager->get(req);
    connect(reply, &QNetworkReply::readyRead, this ,[=]{
        if (reply->error() == QNetworkReply::NoError){
            getDataFromDisk(token);
        }

    });
}

void YandexApi::uploadData(const QByteArray &data, const QString &token)
{
    auto req = QNetworkRequest();
    req.setRawHeader("Authorization",QString( "OAuth " ).append(token ).toUtf8());
    req.setUrl(QString(DISK_METADATA));
    auto reply = apiManager->get(req);
    connect(reply, &QNetworkReply::readyRead, this ,[=]{
        if (reply->error() == QNetworkReply::NoError){
            setDataToDisk(data,token);
        }

    });
}

void YandexApi::getOAuthToken()
{



}

void YandexApi::getDataFromDisk(const QString &token)
{
    auto req = QNetworkRequest();
    req.setRawHeader("Authorization",QString( "OAuth " ).append(token ).toUtf8());
    req.setUrl(QString(DISK_DOWNLOAD));
    auto reply = apiManager->get(req);
    connect(reply, &QNetworkReply::readyRead, this ,[=]{
        if (reply->error() == QNetworkReply::NoError){
            auto data  = reply->readAll();
            auto req1 = QNetworkRequest();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isNull()){
                return;
            }
            auto root = doc.object();
            req1.setUrl(root.value("href").toString());
            auto reply1 = apiManager->get(req);
            connect(reply1, &QNetworkReply::readyRead, this ,[=]{
                auto data1  = reply1->readAll();

                emit dataGranted(data);

            });
        } else {
                auto data  = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
                if (doc.isNull()){
                return;
                }
                auto root = doc.object();
                if (root.value("error").toString() =="DiskNotFoundError" ){
                 emit emptyDiskData();
                }

        }


    });

}

void YandexApi::setDataToDisk(const QByteArray &data, const QString &token)
{
    auto req = QNetworkRequest();
    req.setUrl(QString(DISK_UPLOAD));
    req.setRawHeader("Authorization",QString( "OAuth " ).append(token ).toUtf8());
    auto reply = apiManager->get(req);
    connect(reply, &QNetworkReply::readyRead, this ,[=]{
        if (reply->error() == QNetworkReply::NoError){
            auto data  = reply->readAll();
            auto req1 = QNetworkRequest();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isNull()){
                return;
            }
            auto root = doc.object();
            req1.setUrl(root.value("href").toString());
            auto reply1 = apiManager->put(req, data );
            connect(reply1, &QNetworkReply::readyRead, this ,[=]{
                if (reply1->error() == QNetworkReply::NoError){

                    emit dataUploaded();
                }

            });
        }


    });

}

void YandexApi::replyFinished(QNetworkReply *reply)
{
//    auto data  = reply->readAll();
    qDebug()<< reply->errorString();
//    qDebug() <<QString(data);
}

void YandexApi::codeRecieved(const QString &code)
{
    qDebug() <<"post";
    auto req = QNetworkRequest();
    req.setUrl(QString(OUATHFINISH_METHOD));
    QUrlQuery postData;
    postData.addQueryItem("client_id", "d393faa55762423388db7d8946a1d719");
    postData.addQueryItem("grant_type","authorization_code");
    postData.addQueryItem("code",code);
    postData.addQueryItem("client_secret","3dab55796f6c45d5a6ab7fa359235692");


//    auto str = QString("client_id=597189b7fa1b4c04a024e4ec2bee105d&"
//               "grant_type=authorization_code&"
//               "code=%1&"
//               "client_secret=7b20443819fe4594b1768ea2044711ae");
    auto reply = apiManager->post(req, postData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::readyRead, this ,[=]{
        auto data  = reply->readAll();
        qDebug() <<QString(data);
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()){
            return;
        }
        auto root = doc.object();
        qDebug() << root.value("access_token").toString();
        emit tokenGranted( root.value("access_token").toString());

    });
}
