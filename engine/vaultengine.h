#ifndef VAULTENGINE_H
#define VAULTENGINE_H

#include"cryptlib.h"
#include "aes.h"
#include "modes.h"
#include "filters.h"
#include "hex.h"
#include "sha.h"
#include "base64.h"
#include <QString>
#include <QMap>
#include "fileprovider/fileprovider.h"
#include <ccm.h>
#include <scrypt.h>
#include <osrng.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cryptlib.h>
#include <hex.h>
#include <filters.h>
#include <randpool.h>
#include <QDebug>
#include <iostream>
#include <files.h>
#include <array>
#include <QTimer>
#include "api/yandexapi.h"



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
    struct EncryptedData{
         std::vector<CryptoPP::byte> key;
          std::vector<CryptoPP::byte> password;
           CryptoPP::byte iv[32];
    };


     VaultEngine(QObject *parent = nullptr);
    ~VaultEngine();
    void checkMasterPassword(QString password);
    void setNewMasterPassword(QString newPassword);
    void appendPassword(PasswordData newData);
    void deletePassword(QString key);
    const QMap<QString, PasswordData> &getPasswordMap() const;
    void blockVault();
    Status getCurrentStatus() const;
void writePasswords(QString password);
private:
    VaultSettings * m_settings;
    CryptoPP::byte masterKey[32];
    CryptoPP::byte masterKeyHash[32];
    FileProvider *fileProvider;
    CryptoPP::byte masterPasswordSalt[32];
    CryptoPP::byte hashSalt[32];
    QTimer idleTimer;
    YandexApi *m_yandexApi;
    int attemptCounter = 3;
    QMap<QString, PasswordData> passwordMap;
    QVector<EncryptedData> encryptedData;
    Status currentStatus = IdleClosed;
    void syncData();
    void changeStatus(Status newStatus);
    void decryptPasswords();
    void encryptPasswords();
    void closeVault();
    QString decryptAES(std::vector<CryptoPP::byte> cyphertext, CryptoPP::byte* key, CryptoPP::byte* iv);
    std::vector<CryptoPP::byte> encryptAES(std::vector<CryptoPP::byte> plaintext, CryptoPP::byte* key, CryptoPP::byte* iv);
    static bool checkEqual(CryptoPP::byte* arr1, int size1, CryptoPP::byte* arr2, int size2);
    static int cpBytesToArray(CryptoPP::byte* dest, QByteArray src, int size, int offset=0);
    static void cpBytesToVec(std::vector<CryptoPP::byte>& dest, QByteArray src, int size, int offset=0);
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
};

#endif // VAULTENGINE_H
