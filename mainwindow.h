#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include "widgets/requirepasswordwidget.h"
#include "engine/vaultengine.h"
#include "widgets/passwordswidget.h"
#include <QStyle>
#include <QShowEvent>
#include "widgets/requirepassworddialog.h"
#include "settings/storagesettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    VaultEngine *vaultEngine;
    PasswordsWidget *passwordsWidget;
    RequirePasswordWidget *requirePasswordWidget;
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    StorageSettings *storageSettings = nullptr;
    bool firstShow = true;


protected:
    void closeEvent(QCloseEvent *) override;
    void showEvent(QShowEvent*) override;
private slots:
    void onNewPasswordCreated(QString newPassword);
    void onVaultStatusChanged(int status);
    void onPasswordEntered(QString password);
    void onNewAccountRequest(PasswordData*);
    void onDeleteAccountRequest(QString key);
    void onCloseVaultRequest();
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void onStorageSettingsOpen();
    void onMessageRecieved(const QString& title, const QString& message);
};
#endif // MAINWINDOW_H
