#include "crypto_utils.h"
#include <fstream>
#include <filesystem>
#include <hex.h>
#include <aes.h>
#include <modes.h>
#include <filters.h>
#include <randpool.h>
#include <ccm.h>
#include <scrypt.h>
#include <osrng.h>


crypto_utils::CryptoEngine::CryptoEngine() {

}
crypto_utils::CryptoEngine::~CryptoEngine() {

}

void crypto_utils::CryptoEngine::choose_type(const TypeCryptographic & type) {
    m_type = type;
}

bool crypto_utils::CryptoEngine::push_to_queue(const std::string & filename){
    if (std::filesystem::exists(filename)) {
        m_files_queue.push(filename);
        return true;
    }
    return false;
}

bool crypto_utils::CryptoEngine::push_to_queue(const std::vector<CryptoPP::byte> &data)
{
    if (!data.empty()) {
        m_data_queue.push(data);
        return true;
    }
    return false;
}
bool crypto_utils::CryptoEngine::process_queue(){
    uint32_t count_files = 0;
    for( ;!m_files_queue.empty(); ) {
        switch (m_type) {
            case XOR:
                if (!process_xor(m_files_queue.front())) {
                    return false;
                }
                break;
            case shuffle:
                break;
            case shift:
                if (!process_shift(m_files_queue.front())) {
                    return false;
                }
                break;

            default:
                return false;

        }
        m_files_queue.pop();
        count_files++;
    }
    std::cout << "Processed " << count_files << " files" << std::endl;
    uint32_t count_data = 0;
    for( ;!m_data_queue.empty(); ) {
        switch (m_type) {
        case AES:{
            auto processed_data = process_aes(m_data_queue.front(), m_aes_key, m_aes_iv);
            if(processed_data.has_value()) {
                m_results_queue.push(processed_data.value());
            }
            break;
        }
        default:
            return false;
        }
        m_files_queue.pop();
        count_files++;
    }
    std::cout << "Processed " << count_data << " data" << std::endl;
    return true;
}

bool crypto_utils::CryptoEngine::process_xor(const std::string &filename, unsigned char base) {
    std::ifstream file_input(filename, std::ios::binary);
    std::string new_filename(filename + "_tmp");
    std::ofstream file_output(new_filename, std::ios::binary);
    char buffer[1024];
    while (file_input.read(buffer, sizeof(buffer))) {
        for (char & i : buffer) {
            i ^= base;
        }
        file_output.write(buffer, sizeof(buffer));
    }
    for (int i = 0; i < file_input.gcount(); i++) {
        buffer[i] ^= base;
    }
    file_output.write(buffer, file_input.gcount());
    file_input.close();
    file_output.close();
    if (!std::filesystem::remove(filename)) {
        std::cout << " crypto_utils::CryptoEngine::process_xor \n failed to remove " << filename << std::endl;
        return false;
    }
    std::filesystem::rename(new_filename, filename);
    return true;
}

bool crypto_utils::CryptoEngine::process_shift(const std::string &filename, unsigned char base) {
    std::ifstream file_input(filename, std::ios::binary);
    std::string new_filename(filename + "_tmp");
    std::ofstream file_output(new_filename, std::ios::binary);
    char buffer[1024];
    if (m_action == Decrypt) {base = -base;};
    while (file_input.read(buffer, sizeof(buffer))) {
        for (char & i : buffer) {
            i += base;
        }
        file_output.write(buffer, sizeof(buffer));
    }
    for (int i = 0; i < file_input.gcount(); i++) {
        buffer[i] += base;
    }
    file_output.write(buffer, file_input.gcount());
    file_input.close();
    file_output.close();
    if (!std::filesystem::remove(filename)) {
        std::cout << " crypto_utils::CryptoEngine::process_shift \n failed to remove " << filename << std::endl;
        return false;
    }
    std::filesystem::rename(new_filename, filename);
    return true;
}



std::optional<std::vector<CryptoPP::byte>> crypto_utils::CryptoEngine::process_aes(std::vector<CryptoPP::byte> data, CryptoPP::byte *key, CryptoPP::byte *iv)
{
    using namespace CryptoPP;
    switch(m_action) {
    case Encrypt: {
        std::vector<byte> plain, cipher, recover;
        plain.assign(data.begin(),data.end());
        std::string encryptedtext;
        std::string plaintexT;
        ArraySource(plain.data(),plain.size(),1,new HexEncoder(new StringSink(plaintexT)));
        try{
            CBC_Mode<CryptoPP::AES>::Encryption enc;
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
    case Decrypt: {
        std::vector<byte> plain, cipher, recover;
        cipher.assign(data.begin(),data.end());
        std::string decryptedtext;
        try{
            CBC_Mode<CryptoPP::AES>::Decryption dec;
            dec.SetKeyWithIV(key, AES::MAX_KEYLENGTH, iv, AES::BLOCKSIZE);
            recover.resize(cipher.size());
            ArraySink rs(&recover[0], recover.size());
            ArraySource(cipher.data(), cipher.size(), true,
                        new StreamTransformationFilter(dec, new Redirector(rs)));
        }
        catch(const Exception& e)
        {
            std::cerr << e.what() << std::endl;
            return {};
        }
        return recover;
    }
    }
}

void crypto_utils::CryptoEngine::choose_action(const crypto_utils::TypeAction &type) {
    m_action = type;
}


namespace crypto_utils {

void CryptoEngine::set_aes_iv(CryptoPP::byte *new_aes_iv)
{
    m_aes_iv = new_aes_iv;
}


void CryptoEngine::set_aes_key(CryptoPP::byte *new_aes_key)
{
    m_aes_key = new_aes_key;
}

}
