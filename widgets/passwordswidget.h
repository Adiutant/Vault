#ifndef PASSWORDSWIDGET_H
#define PASSWORDSWIDGET_H

#include <QListWidget>
#include "globals.h"
#include <QHBoxLayout>
#include <QPushButton>
#include "newaccountdialog.h"
#include "passwordwidget.h"
#include <QLabel>

class PasswordsWidget : public QListWidget
{
    Q_OBJECT
public:
    PasswordsWidget(QMap<QString, PasswordData>, QWidget* parent);
    void reloadData(QMap<QString, PasswordData>);

private slots:
    void onNewAccountDialog();
signals:
    void newAccountRequest(PasswordData*);
    void deleteAccountRequest(QString);

};

#endif // PASSWORDSWIDGET_H
