#ifndef PASSWORDWIDGET_H
#define PASSWORDWIDGET_H

#include <QWidget>
#include "globals.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QLabel>

class PasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordWidget(PasswordData data,QWidget *parent = nullptr);
private:
    QPushButton *copyButton;
    QPushButton *deleteButton;
    PasswordData passwordData;
    QLabel *keyLabel;
    QLabel *passwordLabel;
private slots:
    void onCopyClicked();
signals:
    void deleteRequest(QString);
};

#endif // PASSWORDWIDGET_H
