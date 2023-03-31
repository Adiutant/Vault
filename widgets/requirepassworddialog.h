#ifndef REQUIREPASSWORDDIALOG_H
#define REQUIREPASSWORDDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "authlineedit.h"

class RequirePasswordDialog : public QDialog
{
    Q_OBJECT
public:
    RequirePasswordDialog(const QString& title = "Требуется пароль",QWidget *widget = nullptr);
    QString getPassword();
private:
    AuthLineEdit* passLine;
    QPushButton* acceptPB;
private slots:
    void onAccept();

};

#endif // REQUIREPASSWORDDIALOG_H
