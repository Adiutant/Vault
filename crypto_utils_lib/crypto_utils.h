#pragma once
#include <string>
#include <iostream>
#include <queue>
#include <cryptlib.h>
#include <variant>
#include <optional>

namespace crypto_utils {
    enum TypeCryptographic {
      XOR,
      shift,
      shuffle,
      AES
    };
    enum TypeAction {
        Encrypt,
        Decrypt
    };
    class CryptoEngine {
    public:
        CryptoEngine();
        ~CryptoEngine();
        void choose_type(const TypeCryptographic & type);
        void choose_action(const TypeAction & type);
        bool push_to_queue(const std::string & filename);
        bool push_to_queue(const std::vector<CryptoPP::byte> & data);
        bool process_queue();
        void set_aes_key(CryptoPP::byte *new_aes_key);

        void set_aes_iv(CryptoPP::byte *new_aes_iv);

    private:
        TypeCryptographic m_type;
        TypeAction m_action;
        bool process_xor(const std::string & filename, unsigned char base = 0x56);
        bool process_shift(const std::string & filename, unsigned char base = 0x56);
        std::optional<std::vector<CryptoPP::byte>> process_aes(std::vector<CryptoPP::byte> data, CryptoPP::byte* key, CryptoPP::byte* iv);
        std::queue<std::string> m_files_queue;
        std::queue<std::vector<CryptoPP::byte>> m_data_queue;
        std::queue<std::vector<CryptoPP::byte>> m_results_queue;
        CryptoPP::byte* m_aes_key;
        CryptoPP::byte* m_aes_iv;

    };
}
