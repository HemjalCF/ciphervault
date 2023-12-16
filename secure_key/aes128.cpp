#include "aes128.hh"

AES128::AES128()
{

}

void AES128::encryptAES(const unsigned char *plaintext, const unsigned char *key, unsigned char *ciphertext)
{
    AES_KEY aesKey;
    AES_set_encrypt_key(key, 128, &aesKey);
    AES_encrypt(plaintext, ciphertext, &aesKey);
}

void AES128::decryptAES(const unsigned char *ciphertext, const unsigned char *key, unsigned char *decryptedText)
{
    AES_KEY aesKey;
    AES_set_decrypt_key(key, 128, &aesKey);
    AES_decrypt(ciphertext, decryptedText, &aesKey);
}
