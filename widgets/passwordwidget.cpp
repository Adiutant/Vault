#include "passwordwidget.h"

PasswordWidget::PasswordWidget(PasswordData data, QWidget *parent)
    : QWidget{parent}, passwordData{data}
{
    QHBoxLayout  * hLay =  new  QHBoxLayout () ;
    QVBoxLayout  * vLay =  new  QVBoxLayout () ;
    QVBoxLayout  * vLay2 =  new  QVBoxLayout () ;

    copyButton =  new QPushButton ( "Скопировать" ) ;
    deleteButton =  new QPushButton ( "Удалить" ) ;
    keyLabel = new QLineEdit(data.key);
    passwordLabel = new QLineEdit("******");
    passwordLabel->adjustSize();
    keyLabel->setReadOnly(true);
    passwordLabel->setReadOnly(true);
    visibilityChB =  new QCheckBox("Показать");
    visibilityChB->setChecked(false);
    vLay->addWidget(keyLabel);
    passwordLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    vLay->addWidget(passwordLabel);
    hLay->addLayout(vLay,100) ;
    vLay2->addWidget(copyButton);
    vLay2->addWidget(visibilityChB);
    hLay->addLayout(vLay2,100) ;
    hLay->setAlignment(vLay2, Qt::AlignCenter);
    hLay->addWidget(deleteButton,100);
    hLay->setAlignment(deleteButton, Qt::AlignRight);
    contextAction = new QAction(passwordData.key);

    QWidget  * buttonWidget =  new  QWidget (this ) ;
    buttonWidget-> setLayout ( hLay ) ;
    connect (copyButton, &QPushButton::clicked, this, &PasswordWidget::onCopyClicked);
    connect (deleteButton, &QPushButton::clicked, this,[=]{
        deleteRequest(passwordData.key);
    });
    connect (visibilityChB, &QPushButton::toggled, this, &PasswordWidget::onVisibilityChanged);
    connect (contextAction, &QAction::triggered, this, &PasswordWidget::onCopyClicked);
    setMaximumWidth(390);
}

QAction *PasswordWidget::getContextAction() const
{
    return contextAction;
}

void PasswordWidget::onCopyClicked()
{
    QClipboard* c = QApplication::clipboard();
    c->setText(passwordData.password);
}

void PasswordWidget::onVisibilityChanged(bool visible)
{
    if (visible){
        passwordLabel->setText(passwordData.password);

    } else{
        passwordLabel->setText("******");
    }
    passwordLabel->adjustSize();
}
