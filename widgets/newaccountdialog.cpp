#include "newaccountdialog.h"

NewAccountDialog::NewAccountDialog(QWidget *parent) :QDialog{parent}
{
    auto layout = new QVBoxLayout(this);
    keyLine = new AuthLineEdit("Ресурс / логин",this);
    passLine = new AuthLineEdit("Пароль",this);
    acceptPB = new QPushButton("Добавить",this);
    layout->addWidget(keyLine);
    layout->addWidget(passLine);
    layout->addWidget(acceptPB);
    connect (acceptPB, &QPushButton::clicked, this, &NewAccountDialog::onAccept);

}

int NewAccountDialog::exec()
{
    return QDialog::exec();
}

PasswordData *NewAccountDialog::newAccount()
{
    account = new PasswordData{keyLine->text(),passLine->text()};
    return account;
}



void NewAccountDialog::onAccept()
{
    if (!keyLine->text().isEmpty() && !passLine->text().isEmpty()){
        done(1);
    }
}
