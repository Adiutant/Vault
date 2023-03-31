#ifndef STORAGESETTINGS_H
#define STORAGESETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include<QDesktopWidget>
#include <QStyle>
#include <QLabel>
#include <QMessageBox>
#include "globals.h"

class StorageSettings : public QWidget
{
    Q_OBJECT
public:
    explicit StorageSettings(QWidget *parent = nullptr);
private:
    QPushButton *connectToYaDisk;

private slots:
    void handleSettingsChanged(const QString &key);


signals:
    void connectToYaDiskRequest();
};

#endif // STORAGESETTINGS_H
