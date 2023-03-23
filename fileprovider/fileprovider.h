#ifndef FILEPROVIDER_H
#define FILEPROVIDER_H
#include<cryptlib.h>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QByteArray>
#include "globals.h"

class FileProvider
{
private:
public:
    FileProvider(QString fileName, CryptoPP::byte* hashSalt [32], CryptoPP::byte* masterPasswordSalt , QMap<CryptoPP::byte*,PasswordData>& passwordsMap, CryptoPP::byte*& passwordsHash);
    ~FileProvider() = default;
    void clearFile(QString filename);

};

#endif // FILEPROVIDER_H
