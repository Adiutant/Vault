#ifndef SCHEMEEVENTFILTER_H
#define SCHEMEEVENTFILTER_H

#include <QObject>
#include <QDebug>
#include <QFileOpenEvent>
#include <QEvent>
#include <QString>
#include <QUrl>
class SchemeEventFilter : public QObject
{
    Q_OBJECT
public:
    SchemeEventFilter();

public slots:
     void handleURL(const QString &url);
    protected:
    bool eventFilter(QObject *object, QEvent *event);
signals:
    void incomingURL(const QString &path);

};

#endif // SCHEMEEVENTFILTER_H
