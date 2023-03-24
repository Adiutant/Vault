#include "passwordswidget.h"



PasswordsWidget::PasswordsWidget(QMap<QString, PasswordData> data, QWidget *parent) :QListWidget{parent},m_parent{parent}
{
    passwordsMenu  = new QMenu(parent);
    reloadData(data);
}

void PasswordsWidget::reloadData(QMap<QString, PasswordData> data)
{
    clear();
    passwordsMenu->clear();
    for (auto key : data.keys()){
        auto psw = new PasswordWidget(data[key],this);
        QListWidgetItem  * listItem = new  QListWidgetItem ( this) ;
        listItem->setSizeHint(QSize(this->width()-10,70));
        connect(psw,&PasswordWidget::deleteRequest, this,&PasswordsWidget::deleteAccountRequest );
        setItemWidget(listItem,psw);
        passwordsMenu->addAction(psw->getContextAction());
    }
    QAction * viewWindow = new QAction(tr("Развернуть окно"), this);
    QAction * quitAction = new QAction(tr("Выход"), this);

    /* подключаем сигналы нажатий на пункты меню к соответсвующим слотам.
     * Первый пункт меню разворачивает приложение из трея,
     * а второй пункт меню завершает приложение
     * */
    connect(viewWindow, SIGNAL(triggered()), m_parent, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), m_parent, SLOT(close()));

    passwordsMenu->addAction(viewWindow);
    passwordsMenu->addAction(quitAction);


    QHBoxLayout  * hLay =  new  QHBoxLayout () ;

    QPushButton  * newButton =  new QPushButton ( "Добавить аккаунт" ) ;

    hLay->addWidget ( newButton ) ;

    QWidget  * buttonWidget =  new  QWidget (this ) ;
    buttonWidget-> setLayout ( hLay ) ;
    QListWidgetItem  * listItem = new  QListWidgetItem ( this) ;
    listItem->setSizeHint(QSize(this->width()-10,50));
    setItemWidget(listItem,buttonWidget);
    connect(newButton, &QPushButton::clicked, this,&PasswordsWidget::onNewAccountDialog );
}

QMenu *PasswordsWidget::getPasswordsMenu() const
{
    return passwordsMenu;
}



void PasswordsWidget::onNewAccountDialog()
{
    auto newAccountDialog = new NewAccountDialog(this);
    if(newAccountDialog->exec()){
        emit newAccountRequest(newAccountDialog->newAccount());
    }
    delete newAccountDialog;
}
