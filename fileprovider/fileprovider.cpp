#include "fileprovider.h"

//FileProvider::FileProvider(QString fileName, QByteArray &hashSalt, QByteArray &masterPasswordSalt, QMap<QByteArray, PasswordData> &passwordsMap, QByteArray &passwordsHash)
//{
//    QFile file(fileName);
//    file.open(QIODevice::ReadWrite);
//    QTextStream in(&file);
//    int i = 0;
//    QByteArray key;
//    QByteArray iv;
//    QByteArray password;
//    int k = 1;
//       while (!in.atEnd())
//       {
//           if (i==0){
//               passwordsHash =  QByteArray::fromBase64(in.read(32).toLocal8Bit());
//           }else if (i==1){
//               hashSalt =  QByteArray::fromBase64(in.read(32).toLocal8Bit());
//           }else if (i==2) {
//               masterPasswordSalt =  QByteArray::fromBase64(in.read(32).toLocal8Bit());
//           }else if (k == 1 ){
//               key = QByteArray::fromBase64(in.read(32).toLocal8Bit());
//               k++;
//           } else if (k == 2){
//              password = QByteArray::fromBase64(in.read(32).toLocal8Bit());
//              k++;
//           } else if (k == 3){
//               passwordsMap.insert(key,{password, QByteArray::fromBase64(in.read(32).toLocal8Bit())});
//               k=1;
//           }
//           i++;

//       }
//       file.close();

//}


FileProvider::FileProvider(QString fileName, CryptoPP::byte *hashSalt[], CryptoPP::byte *masterPasswordSalt, QMap<CryptoPP::byte *, PasswordData> &passwordsMap, CryptoPP::byte *&passwordsHash)
{

}

void FileProvider::clearFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.close();
}


