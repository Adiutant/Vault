#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    requirePasswordWidget =  new RequirePasswordWidget(this);
    ui->mainLayout->setWidget(0,QFormLayout::SpanningRole,requirePasswordWidget );
    connect(requirePasswordWidget, &RequirePasswordWidget::newPasswordCreated, this, &MainWindow::onNewPasswordCreated);
     connect(requirePasswordWidget, &RequirePasswordWidget::passwordEntered, this, &MainWindow::onPasswordEntered);
    vaultEngine = new VaultEngine();
    connect(vaultEngine, &VaultEngine::statusChanged, this, &MainWindow::onVaultStatusChanged);
    passwordsWidget = new PasswordsWidget(QMap<QString,PasswordData>(),this);
    passwordsWidget->setVisible(false);
    ui->mainLayout->setWidget(1,QFormLayout::SpanningRole,passwordsWidget );
    connect(passwordsWidget,&PasswordsWidget::newAccountRequest, this,&MainWindow::onNewAccountRequest);
    connect(passwordsWidget,&PasswordsWidget::deleteAccountRequest, this,&MainWindow::onDeleteAccountRequest);
    setFixedSize(size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewPasswordCreated(QString newPassword)
{
    vaultEngine->setNewMasterPassword(newPassword);
}

void MainWindow::onVaultStatusChanged(int status)
{
    switch (status){
        case VaultEngine::IdleOpened:
            requirePasswordWidget->setVisible(false);
            passwordsWidget->setVisible(true);
            passwordsWidget->reloadData(vaultEngine->getPasswordMap());
        break;
    case VaultEngine::IdleClosed:
        requirePasswordWidget->setVisible(true);
        passwordsWidget->setVisible(false);
        passwordsWidget->reloadData(QMap<QString,PasswordData>());
        break;
    }
}

void MainWindow::onPasswordEntered(QString password)
{
    vaultEngine->checkMasterPassword(password);
}

void MainWindow::onNewAccountRequest(PasswordData *data)
{
    vaultEngine->appendPassword(*data);

    auto requirePasswordDialog = new RequirePasswordDialog(this);
    while(vaultEngine->getCurrentStatus() == VaultEngine::Changed){
        if(requirePasswordDialog->exec()){
                vaultEngine->writePasswords(requirePasswordDialog->getPassword());
            }
        }
    delete requirePasswordDialog;

}

void MainWindow::onDeleteAccountRequest(QString key)
{
    vaultEngine->deletePassword(key);

    auto requirePasswordDialog = new RequirePasswordDialog(this);
    while(vaultEngine->getCurrentStatus() == VaultEngine::Changed){
        if(requirePasswordDialog->exec()){
                vaultEngine->writePasswords(requirePasswordDialog->getPassword());
            }
        }
    delete requirePasswordDialog;
}

