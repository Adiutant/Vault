#ifndef VAULTENGINE_H
#define VAULTENGINE_H

#include <QString>
#include <QMap>
#include "fileprovider/fileprovider.h"
#include "api/yandexapi.h"
#include <iostream>
#include <files.h>
#include <array>
#include <QTimer>




class VaultEngine :public QObject
{
    Q_OBJECT

public:
    enum Status{
        IdleClosed,
        IdleOpened,
        Changed,
        ChangeConfirmed,
        Compromized

    };



    VaultEngine(QObject *parent = nullptr);
    ~VaultEngine();
    void checkMasterPassword(QString password);
    void setNewMasterPassword(QString newPassword);
    void appendPassword(PasswordData newData);
    void deletePassword(QString key);
    const QMap<QString, PasswordData> &getPasswordMap() const;
    void blockVault();
    void syncData();
    Status getCurrentStatus() const;
void writePasswords(QString password);
private:
    VaultSettings * m_settings;
    CryptoPP::byte masterKey[32];
    QString tokenPassword;
    CryptoPP::byte masterKeyHash[32];
    data_utils::FileProvider *fileProvider;
    CryptoPP::byte masterPasswordSalt[32];
    CryptoPP::byte hashSalt[32];
    QTimer idleTimer;
    YandexApi *m_yandexApi;
    int attemptCounter = 3;
    QMap<QString, PasswordData> passwordMap;
    QVector<data_utils::EncryptedData> encryptedData;
    Status currentStatus = IdleClosed;

    void changeStatus(Status newStatus);
    void decryptPasswords();
    void encryptPasswords();
    void closeVault();
    QString decryptAES(std::vector<CryptoPP::byte> cyphertext, CryptoPP::byte* key, CryptoPP::byte* iv);
    std::vector<CryptoPP::byte> encryptAES(std::vector<CryptoPP::byte> plaintext, CryptoPP::byte* key, CryptoPP::byte* iv);
    static bool checkEqual(CryptoPP::byte* arr1, int size1, CryptoPP::byte* arr2, int size2);
    QString getToken();
private slots:
    void handleTimer();
    void handleTokenGranted(const QString& token);
    void onDataGranted(const QByteArray& data);
    void onDataUploaded();
    void onEmptyDiskData();
public slots:
    void handleYandexConnectionRequest();

signals:
    void statusChanged(int status);
    void sendMessage(const QString& title, const QString& message);
};

#endif // VAULTENGINE_H
