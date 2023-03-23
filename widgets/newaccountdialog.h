#ifndef NEWACCOUNTDIALOG_H
#define NEWACCOUNTDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include "authlineedit.h"
#include <QVBoxLayout>
#include <QPushButton>
#include"globals.h"
class NewAccountDialog : public QDialog
{
public:
    struct NewAccount{
      QString keyText;
      QString passText;
    };
    NewAccountDialog(QWidget *parent = nullptr);
    int  exec() override;

    PasswordData* newAccount();
public slots:
    AuthLineEdit* keyLine;
    AuthLineEdit* passLine;
    QPushButton* acceptPB;
    PasswordData* account;
    void onAccept();
};

#endif // NEWACCOUNTDIALOG_H
