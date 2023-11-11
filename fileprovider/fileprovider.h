#ifndef FILEPROVIDER_H
#define FILEPROVIDER_H
#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QByteArray>
#include <QVector>
#include <cryptlib.h>

namespace data_utils {
int cpBytesToArray(CryptoPP::byte* dest, QByteArray src, int size, int offset=0);
void cpBytesToVec(std::vector<CryptoPP::byte>& dest, QByteArray src, int size, int offset=0);
void cp_cr_bytes(CryptoPP::byte* dest, CryptoPP::byte* src, int size);

struct EncryptedData{
    std::vector<CryptoPP::byte> key;
    std::vector<CryptoPP::byte> password;
    CryptoPP::byte iv[32];
};
struct FileData {
    CryptoPP::byte master_key_hash[32];
    CryptoPP::byte hash_salt[32];
    CryptoPP::byte master_password_salt[32];
    QVector<EncryptedData> passwords;
};
class FileProvider
{
private:
    FileData m_data;
public:
    FileProvider(const QString &filename);
    FileProvider(const QString &filename, FileData &file_data);
    FileData get_file_data() const;
    ~FileProvider() = default;
    void clear_file(const QString &filename);

};
}


#endif // FILEPROVIDER_H
