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
        connect(vaultEngine, &VaultEngine::sendMessage, this, &MainWindow::onMessageRecieved);
    passwordsWidget = new PasswordsWidget(QMap<QString,PasswordData>(),this);
    passwordsWidget->setVisible(false);
    ui->mainLayout->setWidget(1,QFormLayout::SpanningRole,passwordsWidget );
    connect(passwordsWidget,&PasswordsWidget::newAccountRequest, this,&MainWindow::onNewAccountRequest);
    connect(passwordsWidget,&PasswordsWidget::deleteAccountRequest, this,&MainWindow::onDeleteAccountRequest);
    connect(passwordsWidget,&PasswordsWidget::closeVaultRequest, this,&MainWindow::onCloseVaultRequest);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/res/images/icon.png"));
        trayIcon->setToolTip("Vault" "\n"
                             "Нажмите правой кнопкой мыши");


                   trayIcon->setContextMenu(passwordsWidget->getPasswordsMenu());
                   trayIcon->show();
           /* Также подключаем сигнал нажатия на иконку к обработчику
            * данного нажатия
            * */
           connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                   this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
           setWindowTitle( QCoreApplication::applicationName() );
        setWindowIcon(QIcon(":/res/images/icon.png"));
            setFixedSize(size());


    connect(ui->storageAction,&QAction::triggered,this, &MainWindow::onStorageSettingsOpen );
}

MainWindow::~MainWindow()
{
    delete ui;
}



/* Метод, который обрабатывает событие закрытия окна приложения
 * */
void MainWindow::closeEvent(QCloseEvent * event)
{
    /* Если окно видимо и чекбокс отмечен, то завершение приложения
     * игнорируется, а окно просто скрывается, что сопровождается
     * соответствующим всплывающим сообщением
     */
    if(this->isVisible()){
        event->ignore();
        this->hide();
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

        trayIcon->showMessage("Tray Program",
                              tr("Приложение свернуто в трей. Для того чтобы, "
                                     "развернуть окно приложения, щелкните по иконке приложения в трее"),
                              icon,
                              2000);
    }
}

void MainWindow::showEvent(QShowEvent * event)
{
    QMainWindow::showEvent(event);
    if (firstShow){

        vaultEngine->syncData();
        firstShow = false;
    }

}


/* Метод, который обрабатывает нажатие на иконку приложения в трее
 * */
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
            /* иначе, если окно видимо, то оно скрывается,
             * и наоборот, если скрыто, то разворачивается на экран
             * */
            if(!this->isVisible()){
                this->show();
            } else {
                this->hide();
            }

        break;
    default:
        break;
    }
}

void MainWindow::onStorageSettingsOpen()
{
    if (storageSettings == nullptr){
        storageSettings = new StorageSettings(this);
        connect(storageSettings, &StorageSettings::connectToYaDiskRequest, vaultEngine, &VaultEngine::handleYandexConnectionRequest);

    }
    storageSettings->show();
}

void MainWindow::onMessageRecieved(const QString &title, const QString &message)
{
    qDebug()<< message;
    QMessageBox::information(this,title,message, QMessageBox::Ok);
}

void MainWindow::onNewPasswordCreated(QString newPassword)
{
    vaultEngine->setNewMasterPassword(newPassword);
}

void MainWindow::onVaultStatusChanged(int status)
{
    switch (status){
        case VaultEngine::IdleOpened:
            requirePasswordWidget->clearField();
            requirePasswordWidget->setVisible(false);
            passwordsWidget->setVisible(true);
            passwordsWidget->reloadData(vaultEngine->getPasswordMap());
        break;
    case VaultEngine::IdleClosed:
            requirePasswordWidget->setVisible(true);
            passwordsWidget->setVisible(false);
            passwordsWidget->reloadData(QMap<QString,PasswordData>());
            this->show();
        break;
    }

    trayIcon->show();
}

void MainWindow::onPasswordEntered(QString password)
{
    vaultEngine->checkMasterPassword(password);
}

void MainWindow::onNewAccountRequest(PasswordData *data)
{
    vaultEngine->appendPassword(*data);

    auto requirePasswordDialog = new RequirePasswordDialog("Введите мастер-пароль",this);
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

    auto requirePasswordDialog = new RequirePasswordDialog("Введите мастер-пароль",this);
    while(vaultEngine->getCurrentStatus() == VaultEngine::Changed){
        if(requirePasswordDialog->exec()){
                vaultEngine->writePasswords(requirePasswordDialog->getPassword());
            }
        }
    delete requirePasswordDialog;
}

void MainWindow::onCloseVaultRequest()
{
    if (vaultEngine->getCurrentStatus() == VaultEngine::IdleOpened){
        vaultEngine->blockVault();
    }
}

