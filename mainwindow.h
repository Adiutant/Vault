#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include "widgets/requirepasswordwidget.h"
#include "engine/vaultengine.h"
#include "widgets/passwordswidget.h"
#include <QStyle>
#include "widgets/requirepassworddialog.h"

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

protected:
    void closeEvent(QCloseEvent *) override;
private slots:
    void onNewPasswordCreated(QString newPassword);
    void onVaultStatusChanged(int status);
    void onPasswordEntered(QString password);
    void onNewAccountRequest(PasswordData*);
    void onDeleteAccountRequest(QString key);
    void iconActivated(QSystemTrayIcon::ActivationReason);
};
#endif // MAINWINDOW_H
