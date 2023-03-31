#include "requirepassworddialog.h"


RequirePasswordDialog::RequirePasswordDialog(const QString &title, QWidget *widget) :QDialog{widget}
{
    passLine = new AuthLineEdit("Мастер пароль", this);
    acceptPB =  new QPushButton("Ok", this);
    auto layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(title));
    layout->addWidget(passLine);
    layout->addWidget(acceptPB);

    connect(acceptPB, &QPushButton::clicked, this, &RequirePasswordDialog::onAccept);
}

QString RequirePasswordDialog::getPassword()
{
    return passLine->text();
}

void RequirePasswordDialog::onAccept()
{
    if (!acceptPB->text().isEmpty()){
        done(1);
    }
}
