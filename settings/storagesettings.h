#ifndef STORAGESETTINGS_H
#define STORAGESETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include<QDesktopWidget>
#include <QStyle>
#include "globals.h"
class StorageSettings : public QWidget
{
    Q_OBJECT
public:
    explicit StorageSettings(QWidget *parent = nullptr);
private:
    QPushButton *connectToYaDisk;

signals:
    void connectToYaDiskRequest();
};

#endif // STORAGESETTINGS_H
