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

void RequirePasswordWidget::clearField()
{
    passwordLine->clear();
    repeatPasswordLine->clear();
}

bool RequirePasswordWidget::checkContainsRequired(QString str)
{
    bool upper = false;
    bool lower = false;
    bool digit = false;
    bool punct = false;
    for (auto ch: str){
        if (ch.isUpper()){
            upper = true;
        }
        if (ch.isLower()){
            lower = true;
        }
        if (ch.isDigit()){
            digit = true;
        }
        if (ch.isPunct()){
            punct = true;
        }

    }
    return upper && lower &&digit &&punct;

}

void RequirePasswordWidget::onNewPasswordCreationStart()
{

    disconnect(applyPBConnection);
    applyPBConnection =  connect (enterPasswordPB, &QPushButton::clicked, this, [=] () {


        if (passwordLine->text() !=repeatPasswordLine->text()){
           QMessageBox::warning(this, "Внимание","Пароли не совпадают",QMessageBox::Ok);
           return;
        }
        if (passwordLine->text().length() < 8|| passwordLine->text().length() > 20){
           QMessageBox::warning(this, "Внимание","Пароль должен быть от 8 до 20 символов",QMessageBox::Ok);
           return;
        }
        if (!checkContainsRequired(passwordLine->text())){
           QMessageBox::warning(this, "Внимание","Пароль должен иметь хотя бы одну букву другого регистра,\n"
                                                 "содержать цифру и любой знак пунктуации",QMessageBox::Ok);
           return;
        }
        emit newPasswordCreated(passwordLine->text());
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


