#include "yandexapi.h"

YandexApi::YandexApi()
{

    syncMutex = new QMutex();
    apiManager = new QNetworkAccessManager(this);
    connect(apiManager, &QNetworkAccessManager::finished,
            this, &YandexApi::replyFinished,Qt::DirectConnection);
    connect (VaultGlobal::schemeHandler.get(), &SchemeEventFilter::incomingURL, this, &YandexApi::codeRecieved);
    apiManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
}

void YandexApi::createConnection()
{

    QDesktopServices::openUrl(OUATHGET_METHOD);
}

void YandexApi::syncData(const QString &token)
{
    getDataFromDisk(token);
}

void YandexApi::uploadData(const QByteArray &data, const QString &token)
{
    setDataToDisk(data,token);
}

void YandexApi::getOAuthToken()
{



}

void YandexApi::getDataFromDisk(const QString &token)
{
    qDebug() <<"getDataFromDisk()";
    auto req = QNetworkRequest();
    req.setRawHeader("Authorization",QString( "OAuth " ).append(token ).toUtf8());
    req.setUrl(QString(DISK_DOWNLOAD));
    auto reply = apiManager->get(req);
    connect(reply, &QNetworkReply::readyRead, this ,[=]{
        qDebug() << "getDataFromDiskErr "<<reply->error();
        if (reply->error() == QNetworkReply::NoError){
            auto data  = reply->readAll();
            auto req1 = QNetworkRequest();
            req1.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
            auto doc1 = QJsonDocument::fromJson(data);
            if (doc1.isNull()){
                return;
            }
            auto root = doc1.object();
            if (root.value("error").toString() =="DiskNotFoundError" ){
                emit emptyDiskData();
                return;
            }
            req1.setUrl(root.value("href").toString());
            qDebug() << root.value("href").toString();
            auto reply1 = apiManager->get(req1);
            connect(reply1, &QNetworkReply::errorOccurred, this ,[=]{

            });
            connect(reply1, &QNetworkReply::readyRead, this ,[=]{
                auto data1  = reply1->readAll();
                emit dataGranted(data1);


            });
            connect(reply1, &QNetworkReply::finished, this ,[=]{

            });

        } else {
            //error
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
        qDebug() << "setDataToDisk "<<reply->error();
        if (reply->error() == QNetworkReply::NoError){
            auto data1  = reply->readAll();
            auto req1 = QNetworkRequest();
            QJsonDocument doc = QJsonDocument::fromJson(data1);
            if (doc.isNull()){
                return;
            }
            auto root = doc.object();
            req1.setUrl(root.value("href").toString());
            auto reply1 = apiManager->put(req1, data );
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
    qDebug()<< reply->errorString();
    reply->deleteLater();
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
