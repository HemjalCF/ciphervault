#ifndef AES128_HH
#define AES128_HH
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iostream>
#include <cstring>

class AES128
{
public:
    AES128();
    void encryptAES(const unsigned char *plaintext, const unsigned char *key, unsigned char *ciphertext);
    void decryptAES(const unsigned char *ciphertext, const unsigned char *key, unsigned char *decryptedText);
};

#endif // AES128_HH
