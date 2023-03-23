#include "requirepasswordwidget.h"

RequirePasswordWidget::RequirePasswordWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout =  new QVBoxLayout(this);
    passwordLine =  new AuthLineEdit("Мастер пароль",this);
    repeatPasswordLine = new AuthLineEdit("Повторите новый пароль", this);
    repeatPasswordLine->setVisible(false);
    enterPasswordPB = new QPushButton("Ок", this);
    createNewPasswordPB =  new QPushButton("Создать новый пароль", this);
    backPB =  new QPushButton("Назад", this);
    backPB->setVisible(false);
    mainLayout->addWidget(passwordLine);
     mainLayout->addWidget(repeatPasswordLine);
    mainLayout->addWidget(enterPasswordPB);
    mainLayout->addWidget(createNewPasswordPB);
    mainLayout->addWidget(backPB);
    connect(createNewPasswordPB, &QPushButton::clicked, this, &RequirePasswordWidget::onNewPasswordCreationStart);
     connect(backPB, &QPushButton::clicked, this, &RequirePasswordWidget::onNewPasswordCreationEnd);
    applyPBConnection =  connect (enterPasswordPB, &QPushButton::clicked, this, [=] () {
        emit passwordEntered(passwordLine->text());
    });



}

RequirePasswordWidget::~RequirePasswordWidget()
{

}

void RequirePasswordWidget::onNewPasswordCreationStart()
{

    disconnect(applyPBConnection);
    applyPBConnection =  connect (enterPasswordPB, &QPushButton::clicked, this, [=] () {
        if (passwordLine->text() ==repeatPasswordLine->text()){
            emit newPasswordCreated(passwordLine->text());
        }
    });
    repeatPasswordLine->setVisible(true);
    createNewPasswordPB->setVisible(false);
    backPB->setVisible(true);

}

void RequirePasswordWidget::onNewPasswordCreationEnd()
{
    disconnect(applyPBConnection);
    connect(createNewPasswordPB, &QPushButton::clicked, this, &RequirePasswordWidget::onNewPasswordCreationStart);
    applyPBConnection =  connect (enterPasswordPB, &QPushButton::clicked, this, [=] () {
        emit passwordEntered(passwordLine->text());
    });
    repeatPasswordLine->setVisible(false);
    createNewPasswordPB->setVisible(true);
    backPB->setVisible(false);

}


