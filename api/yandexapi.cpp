#include "yandexapi.h"

YandexApi::YandexApi()
{

    syncMutex = new QMutex();
    apiManager = new QNetworkAccessManager(this);
    connect(apiManager, &QNetworkAccessManager::finished,
            this, &YandexApi::replyFinished,Qt::DirectConnection);
}

void YandexApi::createConnection()
{
    if(VaultGlobal::SETTINGS->value(YADISK_AUTH).toString().isEmpty()){
        SchemeEventFilter *fl = new SchemeEventFilter();
        QDesktopServices::setUrlHandler("vault", fl,"handleURL");
       QDesktopServices::openUrl(OUATHGET_METHOD);

    }
}

void YandexApi::getOAuthToken()
{

}

void YandexApi::replyFinished(QNetworkReply *reply)
{

}
