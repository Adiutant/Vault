#include "storagesettings.h"

StorageSettings::StorageSettings(QWidget *parent)
    : QWidget{parent}
{
    setWindowFlags(windowFlags() | Qt::Window |Qt::WindowMaximizeButtonHint);
    QDesktopWidget desktop;
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(),
                                    desktop.availableGeometry(this)));
    setWindowTitle("Настройки хранения");
    setAcceptDrops(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QVBoxLayout *syncLayout = new QVBoxLayout();
    QGroupBox *syncGb = new QGroupBox("Синхронизация");
    connectToYaDisk = new QPushButton("Подключить Яндекс Диск",this);

    bool yandexDiskEmpty = !VaultGlobal::SETTINGS->value(YADISK_SET).toBool() || VaultGlobal::SETTINGS->value(YADISK_AUTH).toString().isEmpty();
        qDebug() << "this";
    connectToYaDisk->setEnabled(yandexDiskEmpty);
    syncLayout->addWidget(connectToYaDisk);
    syncGb->setLayout(syncLayout);
    mainLayout->addWidget(syncGb);
    connect (connectToYaDisk, &QPushButton::clicked, this ,[=]{
        emit connectToYaDiskRequest();
    });
    connect(VaultGlobal::SETTINGS, &VaultSettings::valueChanged, this, &StorageSettings::handleSettingsChanged);
}

void StorageSettings::handleSettingsChanged(const QString &key)
{
    if (key == YADISK_SET){
        bool yandexDiskEmpty = !VaultGlobal::SETTINGS->value(YADISK_SET).toBool() || VaultGlobal::SETTINGS->value(YADISK_AUTH).toString().isEmpty();
        qDebug() << "this";
        connectToYaDisk->setEnabled(yandexDiskEmpty);
        QMessageBox::information(this,"Успешно", "Получен токен Яндекса",QMessageBox::Ok);
    }
}
