#include "vaultengine.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <hex.h>
#include <filters.h>
#include <randpool.h>
#include <ccm.h>
#include <scrypt.h>
#include <osrng.h>
#include <memory>
#include <widgets/requirepassworddialog.h>


using namespace CryptoPP;


VaultEngine::VaultEngine(QObject *parent) :QObject{parent}
{
    connect(&idleTimer,&QTimer::timeout, this,&VaultEngine::handleTimer);
    currentStatus = IdleClosed;
    m_settings = new VaultSettings(this);
    VaultGlobal::SETTINGS = m_settings;
    m_yandexApi = new YandexApi();
    connect(m_yandexApi, &YandexApi::dataGranted,this,&VaultEngine::onDataGranted);
    connect(m_yandexApi, &YandexApi::emptyDiskData,this,&VaultEngine::onEmptyDiskData);
    connect(m_yandexApi, &YandexApi::dataUploaded,this,&VaultEngine::onDataUploaded);



}

VaultEngine::~VaultEngine()
{
    delete VaultGlobal::SETTINGS;

}

void VaultEngine::checkMasterPassword(QString password)
{

    auto file_provider = data_utils::FileProvider(FILENAME);

    auto data = file_provider.get_file_data();
    data_utils::cp_cr_bytes(masterPasswordSalt, data.master_password_salt, 32);
    data_utils::cp_cr_bytes(hashSalt, data.hash_salt, 32);
    data_utils::cp_cr_bytes(masterKeyHash, data.master_key_hash, 32);
    encryptedData.append(data.passwords);

    byte* secret  = (unsigned char *)malloc(sizeof(byte*) * password.size());
    memcpy( secret, password.toStdString().c_str() ,password.size());

    Scrypt scrypt;
    scrypt.DeriveKey(masterKey, 32,secret,password.size(),masterPasswordSalt,32,1<<14,8,16);
    std::string masterkeyHex;
     CryptoPP::ArraySource src(masterKey,32,1,new HexEncoder(new StringSink(masterkeyHex)));


       CryptoPP::SHA256 hash;
       byte source[64];
       memcpy(source, hashSalt, 32);
       memcpy(source + 32, masterKey,32);
       byte roundSource[32];
       byte calculatedHash[32];
       CryptoPP::ArraySource foo(source,64, true,
           new CryptoPP::HashFilter(hash,
                new CryptoPP::ArraySink(calculatedHash,32)));
       for (int i =0; i < 5000; i ++){
           memcpy(roundSource,calculatedHash,32);
           CryptoPP::ArraySource foo(roundSource,32, true,
           new CryptoPP::HashFilter(hash,
                new CryptoPP::ArraySink(calculatedHash,32)));
       }

       if (checkEqual(calculatedHash,32, masterKeyHash,32)){
           if (currentStatus == IdleClosed){
            changeStatus(IdleOpened);
           } else if (currentStatus == Changed){
               changeStatus(ChangeConfirmed);
           }
       }
       free (secret);
}

void VaultEngine::setNewMasterPassword(QString newPassword)
{
    AutoSeededRandomPool  rng;
    Scrypt scrypt;
    fileProvider->clear_file(FILENAME);
    byte* secret  = (unsigned char *)malloc(sizeof(byte) * newPassword.length());
    memcpy( secret, newPassword.toStdString().c_str() ,newPassword.length());

    rng.GenerateBlock(masterPasswordSalt,32);

    scrypt.DeriveKey(masterKey, 32,secret,newPassword.length(),masterPasswordSalt,32,1<<14,8,16);
    rng.GenerateBlock(hashSalt,32);


       CryptoPP::SHA256 hash;
       byte source[64];
       memcpy(source, hashSalt, 32);
       memcpy(source + 32, masterKey,32);
       byte roundSource[32];
       CryptoPP::ArraySource foo(source,64, true,
       new CryptoPP::HashFilter(hash,
            new CryptoPP::ArraySink(masterKeyHash,32)));
       for (int i =0; i < 5000; i ++){
           memcpy(roundSource,masterKeyHash,32);
           CryptoPP::ArraySource foo(roundSource,32, true,
           new CryptoPP::HashFilter(hash,
            new CryptoPP::ArraySink(masterKeyHash,32)));
       }

       data_utils::FileData data;
       data_utils::cp_cr_bytes(data.master_key_hash, masterKeyHash, 32);
       data_utils::cp_cr_bytes(data.master_password_salt, masterPasswordSalt, 32);
       data_utils::cp_cr_bytes(data.hash_salt, hashSalt, 32);
       data.passwords.append(encryptedData);
       auto provider = data_utils::FileProvider(FILENAME, data);

       checkMasterPassword(newPassword);
       free (secret);

}

void VaultEngine::appendPassword(PasswordData newData)
{
     passwordMap.insert(newData.key,newData);
     changeStatus(Changed);
}

void VaultEngine::deletePassword(QString key)
{
    passwordMap.remove(key);
    changeStatus(Changed);
}

const QMap<QString, PasswordData> &VaultEngine::getPasswordMap() const
{
    return passwordMap;
}

void VaultEngine::blockVault()
{
    changeStatus(Compromized);
}

VaultEngine::Status VaultEngine::getCurrentStatus() const
{
    return currentStatus;
}

void VaultEngine::writePasswords(QString password)
{
    checkMasterPassword(password);
    if(currentStatus != ChangeConfirmed){
        attemptCounter -=1;
        if (attemptCounter <=0){
            changeStatus(Compromized);
        }
        return;
    }


}

void VaultEngine::syncData()
{
    bool yandexDiskEmpty = !VaultGlobal::SETTINGS->value(YADISK_SET).toBool() || VaultGlobal::SETTINGS->value(YADISK_AUTH).toString().isEmpty();
    if (yandexDiskEmpty){
        return;
    }

    auto token = getToken();
    if (token.isEmpty()){
        return;
    }
    qDebug() << "decrypted token " << token;
    m_yandexApi->syncData(token);
}

void VaultEngine::decryptPasswords()
{
    for (auto item:encryptedData){
        auto key = decryptAES(item.key,masterKey,item.iv);
        auto password = decryptAES(item.password,masterKey,item.iv);
        passwordMap.insert(key,{key,password});
    }
//    for (auto key: passwordMap.keys()){
//        auto data = passwordMap[key];
//        data.password = decryptAES(data.password,masterKey, data.iv);
//        auto newKey = decryptAES(key,masterKey,data.iv);
//        passwordMap.remove(key);
//        passwordMap.insert(newKey,data);
//    }

}

void VaultEngine::encryptPasswords()
{
    encryptedData.clear();
    AutoSeededRandomPool  rng;
    for (auto key: passwordMap.keys()){
        data_utils::EncryptedData res;
        rng.GenerateBlock(res.iv,32);
        std::vector<byte> plain;
        data_utils::cpBytesToVec(plain, key.toLocal8Bit(),key.size(),0);
        auto encKey = encryptAES(plain,masterKey, res.iv);
        res.key.assign(encKey.begin(),encKey.end());
        plain.clear();
        data_utils::cpBytesToVec(plain, passwordMap[key].password.toLocal8Bit(),passwordMap[key].password.size(),0);
        auto encPass = encryptAES(plain,masterKey, res.iv);
        res.password.assign(encPass.begin(),encPass.end());
        encryptedData.append(res);
    }
    fileProvider->clear_file(FILENAME);
    data_utils::FileData data;
    data_utils::cp_cr_bytes(data.master_password_salt, masterPasswordSalt, 32);
    data_utils::cp_cr_bytes(data.hash_salt, hashSalt, 32);
    data_utils::cp_cr_bytes(data.master_key_hash, masterKeyHash, 32);
    data.passwords.append(encryptedData);
    auto provider = data_utils::FileProvider(FILENAME, data);
    passwordMap.clear();
    changeStatus(IdleOpened);

}

void VaultEngine::closeVault()
{
    passwordMap.clear();
    encryptedData.clear();
    tokenPassword.clear();
    idleTimer.stop();
    memset(masterKey,0x00,32);

}

QString VaultEngine::decryptAES(std::vector<byte> cyphertext, byte* key, byte* iv)
{
    std::vector<byte> plain, cipher, recover;
    cipher.assign(cyphertext.begin(),cyphertext.end());
    std::string decryptedtext;
    try{
        CBC_Mode<AES>::Decryption dec;
        dec.SetKeyWithIV(key, AES::MAX_KEYLENGTH, iv, AES::BLOCKSIZE);
        recover.resize(cipher.size());
        ArraySink rs(&recover[0], recover.size());
        ArraySource(cipher.data(), cipher.size(), true,
           new StreamTransformationFilter(dec, new StringSink(decryptedtext)));
    }
    catch(const Exception& e)
    {
       std::cerr << e.what() << std::endl;
       return "error";
    }
    return QString::fromStdString(decryptedtext);
}
std::vector<byte> VaultEngine::encryptAES(std::vector<CryptoPP::byte> plaintext, CryptoPP::byte* key, CryptoPP::byte* iv)
{

    std::vector<byte> plain, cipher, recover;
    plain.assign(plaintext.begin(),plaintext.end());
    std::string encryptedtext;
    std::string plaintexT;
    ArraySource(plain.data(),plain.size(),1,new HexEncoder(new StringSink(plaintexT)));
    try{
        CBC_Mode<AES>::Encryption enc;
          enc.SetKeyWithIV(key, AES::MAX_KEYLENGTH, iv, AES::BLOCKSIZE);

           cipher.resize(plain.size() + AES::BLOCKSIZE);
           ArraySink cs(&cipher[0], cipher.size());

             ArraySource(plain.data(), plain.size(), true,
               new StreamTransformationFilter(enc, new Redirector(cs)));

             // Set cipher text length now that its known
             cipher.resize(cs.TotalPutLength());
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    ArraySource(cipher.data(),cipher.size(),1,new HexEncoder(new StringSink(encryptedtext)));
    return cipher;
}

bool VaultEngine::checkEqual(CryptoPP::byte *arr1, int size1, CryptoPP::byte *arr2, int size2)
{
    if (size1!=size2){
        return false;
    }
    for (int i =0 ;i < size1; i++){
        if (arr1[i] != arr2[i])
        {
            return false;
        }

    }
    return true;
}



QString VaultEngine::getToken()
{
    qDebug() << tokenPassword;
    QString password;
    auto dialog = std::unique_ptr<RequirePasswordDialog>(new RequirePasswordDialog("Токен зашифрован, введите пароль для дешифрования."));
    if (!VaultGlobal::SETTINGS->value(YADISK_USE_ENC).toBool()){
        return VaultGlobal::SETTINGS->value(YADISK_AUTH ).toString();
    } else if (tokenPassword.isEmpty()){

        if (dialog->exec()){
            byte saltBytes[32];
            password = dialog->getPassword();
            byte* secret  = (unsigned char *)malloc(sizeof(byte*) * password.size());
            memcpy( secret, password.toStdString().c_str() ,password.size());
            std::string saltStr = VaultGlobal::SETTINGS->value(YADISK_AUTH_SALT ).toString().toStdString();

            StringSource src(saltStr.c_str(),1,new HexDecoder(new ArraySink(saltBytes, 32)));
            Scrypt scrypt;
            scrypt.DeriveKey(masterKey, 32,secret,password.size(),saltBytes,32,1<<14,8,16);
            free(secret);
            tokenPassword = dialog->getPassword();
        }else{
            emit sendMessage("Ошибка", "Токены зашифрованы, используется локальная копия");

            return QString();

        }
    }else{
        byte saltBytes[32];
        password = tokenPassword;
        byte* secret  = (unsigned char *)malloc(sizeof(byte*) * password.size());
        memcpy( secret, password.toStdString().c_str() ,password.size());
        std::string saltStr = VaultGlobal::SETTINGS->value(YADISK_AUTH_SALT ).toString().toStdString();

        StringSource src(saltStr.c_str(),1,new HexDecoder(new ArraySink(saltBytes, 32)));
        Scrypt scrypt;
        scrypt.DeriveKey(masterKey, 32,secret,password.size(),saltBytes,32,1<<14,8,16);
        free(secret);
    }
    std::vector<byte> tokenBytes;
    std::string tokenPlain;
    //
    std::string keyEncoded;
    ArraySource(masterKey, 32,1,new HexEncoder(new StringSink(keyEncoded)));

    byte iv[32];
    std::string token = VaultGlobal::SETTINGS->value(YADISK_AUTH ).toString().toStdString();

    tokenBytes.resize(token.size()/2);

    std::string ivStr = VaultGlobal::SETTINGS->value(YADISK_AUTH_IV ).toString().toStdString();

    StringSource src(token.c_str(),1,new HexDecoder(new ArraySink(&tokenBytes[0], tokenBytes.size())));
    StringSource ivSt(ivStr.c_str(),1,new HexDecoder(new ArraySink(&iv[0], 32)));

    auto result = decryptAES(tokenBytes,masterKey,iv);
    if (result == "error"){
        emit sendMessage("Ошибка", "Не удалось расшифровать токен, используется локальная копия");
        return QString();
    }

    return result.trimmed();

}

void VaultEngine::handleTimer()
{
    changeStatus(Compromized);
}


void VaultEngine::handleTokenGranted(const QString &token)
{
    disconnect(m_yandexApi, &YandexApi::tokenGranted,this, &VaultEngine::handleTokenGranted );
    QString tokenStr;
    QString ivStr;
    QString saltStr;
    auto dialog = std::unique_ptr<RequirePasswordDialog>(new RequirePasswordDialog("Введите и запомните пароль,\nесли вы хотите использовать шифрование для хранения токена\nесли нет - просто закройте окно"));
    if (dialog->exec()){
        auto password = dialog->getPassword();
        byte* secret  = (unsigned char *)malloc(sizeof(byte*) * password.size());
        memcpy( secret, password.toStdString().c_str() ,password.size());
        AutoSeededRandomPool  rng;
        rng.GenerateBlock(masterPasswordSalt,32);
        Scrypt scrypt;
        scrypt.DeriveKey(masterKey, 32,secret,password.size(),masterPasswordSalt,32,1<<14,8,16);
        std::vector<byte> tokenBytes;
        std::string tokenCrypted;
        std::string ivEncoded;
        std::string saltEncoded;

        tokenBytes.resize(token.size());

        StringSource src(token.toStdString().c_str(),1,new ArraySink(&tokenBytes[0], tokenBytes.size()));
        byte iv[32];
        rng.GenerateBlock(iv,32);
        //
        auto encToken = encryptAES(tokenBytes,masterKey, iv);

        ArraySource(&encToken[0], encToken.size(),1,new HexEncoder(new StringSink(tokenCrypted)));

        ArraySource(iv, 32,1,new HexEncoder(new StringSink(ivEncoded)));
        ArraySource(masterPasswordSalt, 32,1,new HexEncoder(new StringSink(saltEncoded)));
        tokenStr = QString::fromStdString(tokenCrypted);
        ivStr = QString::fromStdString(ivEncoded);
        saltStr = QString::fromStdString(saltEncoded);
        VaultGlobal::SETTINGS->updateValue(YADISK_USE_ENC, true);
        free(secret);
    }
    else{
        tokenStr = token;
        VaultGlobal::SETTINGS->updateValue(YADISK_USE_ENC, false);

    }
    VaultGlobal::SETTINGS->updateValue(YADISK_AUTH, tokenStr);
    VaultGlobal::SETTINGS->updateValue(YADISK_AUTH_IV,ivStr);
    VaultGlobal::SETTINGS->updateValue(YADISK_AUTH_SALT,saltStr);
    VaultGlobal::SETTINGS->updateValue(YADISK_SET, true);
    m_yandexApi->syncData(token);






}

void VaultEngine::onDataGranted(const QByteArray &data)
{
    QFile f(FILENAME);
    f.open(QIODevice::ReadWrite);
    f.write(data);
    f.close();
}

void VaultEngine::onDataUploaded()
{

}

void VaultEngine::onEmptyDiskData()
{
    QFile f(FILENAME);
    f.open(QIODevice::ReadOnly);
    auto data  = f.readAll();
    if (data.isEmpty()){
        return;
    }
    auto token = getToken();
    qDebug() << "decrypted token " << token;
    m_yandexApi->uploadData(data,token);
    f.close();

}

void VaultEngine::handleYandexConnectionRequest()
{
    connect(m_yandexApi, &YandexApi::tokenGranted,this, &VaultEngine::handleTokenGranted );
    m_yandexApi->createConnection();

}

void VaultEngine::changeStatus(Status newStatus)
{ //todo if
    bool yandexDiskEmpty = !VaultGlobal::SETTINGS->value(YADISK_SET).toBool() || VaultGlobal::SETTINGS->value(YADISK_AUTH).toString().isEmpty();
    switch (newStatus) {
        case IdleOpened:
            attemptCounter = 3;
            idleTimer.start(TIMEOUT);
            decryptPasswords();
        break;
    case ChangeConfirmed:
            idleTimer.start(TIMEOUT);
            encryptPasswords();
            if (yandexDiskEmpty){
                return;
            }
            onEmptyDiskData();
            syncData();
        break;
    case Compromized:
            closeVault();
            newStatus = IdleClosed;
        break;

    }

    currentStatus = newStatus;
    emit statusChanged(currentStatus);

}
