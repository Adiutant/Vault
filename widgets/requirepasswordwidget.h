#ifndef REQUIREPASSWORDWIDGET_H
#define REQUIREPASSWORDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMetaObject>
#include <QMessageBox>
#include "widgets/authlineedit.h"
class RequirePasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RequirePasswordWidget(QWidget *parent = nullptr);
    ~RequirePasswordWidget();
private:
    AuthLineEdit *passwordLine;
    QPushButton *enterPasswordPB;
    QPushButton *createNewPasswordPB;
    AuthLineEdit *repeatPasswordLine;
    QPushButton *backPB;
    QMetaObject::Connection applyPBConnection;
    bool checkContainsRequired(QString str);
signals:
    void passwordEntered(QString password);
    void newPasswordCreated(QString newPassword);
public slots:
    void onNewPasswordCreationStart();
    void onNewPasswordCreationEnd();

};

#endif // REQUIREPASSWORDWIDGET_H
