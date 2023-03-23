#ifndef PASSWORDWIDGET_H
#define PASSWORDWIDGET_H

#include <QWidget>
#include "globals.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QLineEdit>
#include <QClipboard>
#include <QCheckBox>
#include <QLabel>

class PasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordWidget(PasswordData data,QWidget *parent = nullptr);
private:
    QPushButton *copyButton;
    QPushButton *deleteButton;
    PasswordData passwordData;
    QCheckBox *visibilityChB;
    QLineEdit *keyLabel;
    QLineEdit *passwordLabel;
private slots:
    void onCopyClicked();
    void onVisibilityChanged(bool visible);
signals:
    void deleteRequest(QString);
};

#endif // PASSWORDWIDGET_H
