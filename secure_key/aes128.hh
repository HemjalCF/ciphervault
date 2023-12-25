#ifndef AES128_HH
#define AES128_HH
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>


class AES128
{
public:
    AES128();
    const unsigned char cypher_key[17] = "fdt34xdrftaslpoj"; // 16-byte key for AES-128
    void encryptAES(const std::string plaintext, std::string &ciphertext);
    void decryptAES(const std::string ciphertext, std::string &decryptedText);
};

#endif // AES128_HH
