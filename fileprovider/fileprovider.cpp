#include "fileprovider.h"
#include <QDebug>
using namespace data_utils;

int data_utils::cpBytesToArray(CryptoPP::byte* dest, QByteArray src, int size, int offset)
{

    if ((offset + size) >src.length()){
        qDebug() << "less" << src.length();
        return -1;
    }
    memcpy(dest,src.data() + offset, size);
    return size;
}

void data_utils::cpBytesToVec(std::vector<CryptoPP::byte> &dest, QByteArray src, int size, int offset)
{
    for (int i =offset ; i < size + offset;i++){
        dest.push_back(src.at(i));
    }

}
data_utils::FileProvider::FileProvider(const QString &filename)
{
    QFile file(filename);
    //fileProvider = new FileProvider(FILENAME,hashSalt, masterPasswordSalt, passwordMap,masterKeyHash);
    file.open(QIODevice::ReadOnly);
    int cursor = 0;
    QByteArray fileBytes = file.readAll();
    unsigned char sizeKey;
    unsigned char sizePass;
    unsigned char passwordsCount;
    cpBytesToArray(m_data.master_key_hash,fileBytes,32,cursor);
    cursor+=32;
    cpBytesToArray(m_data.hash_salt,fileBytes,32,cursor);
    cursor+=32;
    cpBytesToArray(m_data.master_password_salt,fileBytes,32,cursor);
    cursor+=32;
    cpBytesToArray(&passwordsCount,fileBytes,1,cursor);;
    cursor+=1;
    for (int i = 0; i < passwordsCount;i++){
        auto  data = data_utils::EncryptedData{};
        cpBytesToArray(&sizeKey, fileBytes,1,cursor);
        cursor+=1;
        cpBytesToArray(&sizePass, fileBytes,1,cursor);
        cursor+=1;
        cpBytesToVec(data.key,fileBytes,sizeKey,cursor );
        cursor+=sizeKey;
        cpBytesToVec(data.password,fileBytes,sizePass,cursor );
        cursor+=sizePass;
        cpBytesToArray(data.iv,fileBytes,32,cursor);
        cursor+=32;
        m_data.passwords.push_back(data);
    }
    file.close();

}

FileProvider::FileProvider(const QString &filename, FileData &file_data)
{

    QFile file(filename);
    file.open(QIODevice::Append);
    file.write(reinterpret_cast<char *>(&file_data.master_key_hash), 32);
    file.write(reinterpret_cast<char *>(&file_data.hash_salt), 32);
    file.write(reinterpret_cast<char *>(&file_data.master_password_salt), 32);
    char passwordCount;
    char size;
    passwordCount = file_data.passwords.length();
    file.write(reinterpret_cast<char *>(&passwordCount),1);
    for (auto item : file_data.passwords){
        size = item.key.size();
        file.write(reinterpret_cast<char *>(&size), 1);
        size = item.password.size();
        file.write(reinterpret_cast<char *>(&size), 1);
        file.write(reinterpret_cast<char *>(item.key.data()), item.key.size());
        file.write(reinterpret_cast<char *>(item.password.data()), item.password.size());
        file.write(reinterpret_cast<char *>(&item.iv), 32);
    }
    file.close();


}

data_utils::FileData data_utils::FileProvider::get_file_data() const
{
    return m_data;
}

void data_utils::FileProvider::clear_file(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.close();
}



void data_utils::cp_cr_bytes(CryptoPP::byte *dest, CryptoPP::byte *src, int size)
{
    memcpy(dest, src, size);
}
