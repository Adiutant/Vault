#include "passwordswidget.h"



PasswordsWidget::PasswordsWidget(QMap<QString, PasswordData> data, QWidget *parent) :QListWidget{parent}
{
    clear();
    for (auto key : data.keys()){
        auto psw = new PasswordWidget(data[key],this);
        QListWidgetItem  * listItem = new  QListWidgetItem ( this) ;
        listItem->setSizeHint(QSize(200,50));
        connect(psw,&PasswordWidget::deleteRequest, this,&PasswordsWidget::deleteAccountRequest );
        setItemWidget(listItem,psw);
    }

    QHBoxLayout  * hLay =  new  QHBoxLayout () ;

    QPushButton  * newButton =  new QPushButton ( "Добавить аккаунт" ) ;

    hLay->addWidget ( newButton ) ;

    QWidget  * buttonWidget =  new  QWidget (this ) ;
    buttonWidget-> setLayout ( hLay ) ;
    QListWidgetItem  * listItem = new  QListWidgetItem ( this) ;
    listItem->setSizeHint(QSize(200,50));
    setItemWidget(listItem,buttonWidget);
    connect(newButton, &QPushButton::clicked, this,&PasswordsWidget::onNewAccountDialog );
}

void PasswordsWidget::reloadData(QMap<QString, PasswordData> data)
{
    clear();
    for (auto key : data.keys()){
        auto psw = new PasswordWidget(data[key],this);
        QListWidgetItem  * listItem = new  QListWidgetItem ( this) ;
        listItem->setSizeHint(QSize(200,50));
        connect(psw,&PasswordWidget::deleteRequest, this,&PasswordsWidget::deleteAccountRequest );
        setItemWidget(listItem,psw);
    }

    QHBoxLayout  * hLay =  new  QHBoxLayout () ;

    QPushButton  * newButton =  new QPushButton ( "Добавить аккаунт" ) ;

    hLay->addWidget ( newButton ) ;

    QWidget  * buttonWidget =  new  QWidget (this ) ;
    buttonWidget-> setLayout ( hLay ) ;
    QListWidgetItem  * listItem = new  QListWidgetItem ( this) ;
    listItem->setSizeHint(QSize(200,50));
    setItemWidget(listItem,buttonWidget);
    connect(newButton, &QPushButton::clicked, this,&PasswordsWidget::onNewAccountDialog );
}



void PasswordsWidget::onNewAccountDialog()
{
    auto newAccountDialog = new NewAccountDialog(this);
    if(newAccountDialog->exec()){
        emit newAccountRequest(newAccountDialog->newAccount());
    }
    delete newAccountDialog;
}
