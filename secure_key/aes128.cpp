#include "aes128.hh"

AES128::AES128()
{

}

static void str_to_char(std::string in_str, unsigned char *out_char){

    std::copy(in_str.cbegin(), in_str.cend(), out_char);
   // std::cout <<"Conv str to char "<< out_char << '\n';
}

static unsigned char parse_hex(char c)
{
    if ('0' <= c && c <= '9') return c - '0';
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    std::abort();
}

/*
 * In this method first I will convert plain text string to the usigned char array
 * then feed into the encryption algorithm library to get encrypted u_char array
 * finally conevrt the u_char array to the string and return it.
*/

void AES128::encryptAES(const std::string plaintext, std::string &ciphertext)
{
    AES_KEY aesKey;
    AES_set_encrypt_key(cypher_key, 128, &aesKey);
    unsigned char plain_text_char[(sizeof(plaintext)/4)+1];
    str_to_char(plaintext,plain_text_char);
    unsigned char ciphertext_c[AES_BLOCK_SIZE];
    std::stringstream ss;
    AES_encrypt(plain_text_char, ciphertext_c, &aesKey);
    for (int i = 0; i < AES_BLOCK_SIZE; ++i){
        ss << std::hex << std::setw(2) << std::setfill('0') <<(int)ciphertext_c[i];
    }
    ciphertext = ss.str();
    //std::cout <<"Returning C Text "<< ciphertext << '\n';

}

/*
 * In this method first I will convert ciphered text string to the usigned char array
 * then feed into the decryption algorithm library to get decrypted u_char array
 * finally conevrt the u_char array to the string and return it.
*/

void AES128::decryptAES(const std::string ciphertext, std::string &decryptedText)
{
    AES_KEY aesKey;
    AES_set_decrypt_key(cypher_key, 128, &aesKey);
    unsigned char test[AES_BLOCK_SIZE];
    unsigned char decryptedText_c[AES_BLOCK_SIZE];
    for (std::size_t i = 0; i != ciphertext.size() / 2; ++i)
        test[i] = 16 * parse_hex(ciphertext[2 * i]) + parse_hex(ciphertext[2 * i + 1]);
    AES_decrypt(test, decryptedText_c, &aesKey);
    std::stringstream ss;
    for (int i = 0; i < AES_BLOCK_SIZE; ++i)
        ss << decryptedText_c[i];
    decryptedText = ss.str();
    //std::cout <<"Returning D Text: "<< decryptedText << '\n';
}
