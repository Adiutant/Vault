#include "passwordwidget.h"

PasswordWidget::PasswordWidget(PasswordData data, QWidget *parent)
    : QWidget{parent}, passwordData{data}
{
    QHBoxLayout  * hLay =  new  QHBoxLayout () ;
    QVBoxLayout  * vLay =  new  QVBoxLayout () ;

    copyButton =  new QPushButton ( "Скопировать" ) ;
    deleteButton =  new QPushButton ( "Удалить" ) ;
    keyLabel = new QLabel(data.key);
    passwordLabel = new QLabel("******");
    vLay->addWidget(keyLabel);
    vLay->addWidget(passwordLabel);
    hLay->addLayout(vLay) ;
    hLay->addWidget(copyButton);
    hLay->addWidget(deleteButton);


    QWidget  * buttonWidget =  new  QWidget (this ) ;
    buttonWidget-> setLayout ( hLay ) ;
    connect (copyButton, &QPushButton::clicked, this, &PasswordWidget::onCopyClicked);
    connect (deleteButton, &QPushButton::clicked, this,[=]{
        deleteRequest(passwordData.key);
    });
}

void PasswordWidget::onCopyClicked()
{
    QClipboard* c = QApplication::clipboard();
    c->setText(passwordData.password);
}
