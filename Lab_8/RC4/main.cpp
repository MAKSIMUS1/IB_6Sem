#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

class RC4 {
private:
    vector<unsigned char> S;
    int i, j;

public:
    RC4(const vector<unsigned char>& key) {
        S.resize(256);
        for (int i = 0; i < 256; ++i) {
            S[i] = i;
        }

        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + S[i] + key[i % key.size()]) % 256;
            swap(S[i], S[j]);
        }

        this->i = 0;
        this->j = 0;
    }

    unsigned char generateByte() {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        return S[(S[i] + S[j]) % 256];
    }
};

int main() {

    vector<unsigned char> key = { 61, 60, 23, 22, 21, 20 };
    RC4 rc4(key);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 256; ++i) {
        rc4.generateByte();
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start);

    cout << "Generation time: " << duration.count() << " nanoseconds" << endl;

    string message = "KRYSHTAL MAKSIM";
    string encryptedMessage;

    for (unsigned char c : message) {
        unsigned char keyByte = rc4.generateByte();
        encryptedMessage += c ^ keyByte; 
    }

    cout << "Encrypted message: " << encryptedMessage << endl;

    string decryptedMessage;

    RC4 rc4_decryption(key);

    for (char c : encryptedMessage) {
        unsigned char keyByte = rc4_decryption.generateByte();
        decryptedMessage += c ^ keyByte;
    }

    cout << "Decrypted message: " << message << endl;

    return 0;
}