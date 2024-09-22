#include <iostream>
#include <string>
#include <cryptopp/des.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <chrono>

using namespace std;
using namespace CryptoPP;

// Функция для шифрования сообщения с использованием DES-EEE2
string encryptDES(const string& plaintext, const byte* key) {
    string ciphertext;

    try {
        ECB_Mode<DES_EDE2>::Encryption encryption(key, DES_EDE2::DEFAULT_KEYLENGTH);
        StringSource(plaintext, true,
            new StreamTransformationFilter(encryption,
                new StringSink(ciphertext)
            )
        );
    }
    catch (const CryptoPP::Exception& e) {
        cerr << "Error during encryption: " << e.what() << endl;
        exit(1);
    }

    return ciphertext;
}

// Функция для расшифровки сообщения с использованием DES-EEE2
string decryptDES(const string& ciphertext, const byte* key) {
    string decryptedtext;

    try {
        ECB_Mode<DES_EDE2>::Decryption decryption(key, DES_EDE2::DEFAULT_KEYLENGTH);
        StringSource(ciphertext, true,
            new StreamTransformationFilter(decryption,
                new StringSink(decryptedtext)
            )
        );
    }
    catch (const CryptoPP::Exception& e) {
        cerr << "Error during decryption: " << e.what() << endl;
        exit(1);
    }

    return decryptedtext;
}

int main() {
    const byte key[DES_EDE2::DEFAULT_KEYLENGTH] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };

    // Шифруем и расшифровываем сообщение
    string plaintext = "Hello, world!";
    string ciphertext = encryptDES(plaintext, key);
    string decryptedtext = decryptDES(ciphertext, key);

    cout << "Original message: " << plaintext << endl;
    cout << "Encrypted message: " << ciphertext << endl;
    cout << "Decrypted message: " << decryptedtext << endl;

    return 0;
}
