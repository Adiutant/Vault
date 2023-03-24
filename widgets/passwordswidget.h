#ifndef PASSWORDSWIDGET_H
#define PASSWORDSWIDGET_H

#include <QListWidget>
#include "globals.h"
#include <QHBoxLayout>
#include <QPushButton>
#include "newaccountdialog.h"
#include "passwordwidget.h"
#include <QLabel>
#include <QMenu>
class PasswordsWidget : public QListWidget
{
    Q_OBJECT
public:
    PasswordsWidget(QMap<QString, PasswordData>, QWidget* parent);
    void reloadData(QMap<QString, PasswordData>);
    QMenu *getPasswordsMenu() const;

private :
    QMenu* passwordsMenu;
    QWidget* m_parent;
private slots:
    void onNewAccountDialog();
signals:
    void newAccountRequest(PasswordData*);
    void deleteAccountRequest(QString);

};

#endif // PASSWORDSWIDGET_H
