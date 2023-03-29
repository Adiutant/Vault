#include "schemeeventfilter.h"

SchemeEventFilter::SchemeEventFilter()
{

}

bool SchemeEventFilter::eventFilter(QObject *object, QEvent *event)
{
    qDebug()<< event->type();
    if (event->type() == QEvent::FileOpen){
     QFileOpenEvent* fileEvent = static_cast<QFileOpenEvent*>(event);
     if (!fileEvent->url().isEmpty()){
        qDebug() << fileEvent->url().toString();
     }
     return false;
    }else
    {
        // standard event processing
        return QObject::eventFilter(object, event);
    }
}
void SchemeEventFilter::handleURL(const QUrl &url)
{
    qDebug() << url.toString();
   emit incomingURL(url.toString());
}
