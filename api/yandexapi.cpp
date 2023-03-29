#include "yandexapi.h"

YandexApi::YandexApi()
{
    moveToThread(this);
    syncMutex = new QMutex();
}
