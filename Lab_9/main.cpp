#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <bitset>

using namespace std;
using namespace std::chrono;

vector<bitset<100>> generateSuperincreasingSequence(int z);
vector<bitset<100>> calculatePublicKey(const vector<bitset<100>>& sequence);
string encryptMessage(const string& message, const vector<bitset<100>>& publicKey);
string decryptMessage(const string& encryptedMessage, const vector<bitset<100>>& publicKey);

int main() {
    ofstream outputFile("analysis_results.txt");

    vector<int> zValues = { 6, 7, 8 };

    for (int z : zValues) {
        vector<bitset<100>> base64Sequence = generateSuperincreasingSequence(z);

        vector<bitset<100>> publicKey = calculatePublicKey(base64Sequence);

        for (int i = 10; i <= 100; i += 10) {
            auto startEncrypt = high_resolution_clock::now();
            auto startDecrypt = high_resolution_clock::now();

            string encryptedMessage = encryptMessage("KRYSHTAL MAKSIM OLEGOVICH", publicKey);
            string decryptedMessage = decryptMessage(encryptedMessage, publicKey);

            auto stopEncrypt = high_resolution_clock::now();
            auto stopDecrypt = high_resolution_clock::now();

            auto durationEncrypt = duration_cast<microseconds>(stopEncrypt - startEncrypt);
            auto durationDecrypt = duration_cast<microseconds>(stopDecrypt - startDecrypt);

            outputFile << "Z: " << z << ", Key Sequence Size: " << i << endl;
            outputFile << "Encryption Time: " << durationEncrypt.count() << " microseconds" << endl;
            outputFile << "Decryption Time: " << durationDecrypt.count() << " microseconds" << endl;
            outputFile << "---------------------------------------" << endl;
        }
    }

    outputFile.close();

    return 0;
}

// Функция для генерации сверхвозрастающей последовательности
vector<bitset<100>> generateSuperincreasingSequence(int z) {
    vector<bitset<100>> sequence;
    bitset<100> superincreasingElement(1); 
    for (int i = 1; i <= z; ++i) {
        sequence.push_back(superincreasingElement);
        superincreasingElement <<= i; 
    }
    return sequence;
}

// Функция для вычисления нормальной последовательности (открытого ключа)
vector<bitset<100>> calculatePublicKey(const vector<bitset<100>>& superincreasingSequence) {
    vector<bitset<100>> publicKey;
    bitset<100> currentKey(0); 
    for (const auto& element : superincreasingSequence) {
        currentKey |= element; 
        publicKey.push_back(currentKey);
    }
    return publicKey;
}

string encryptMessage(const string& message, const vector<bitset<100>>& publicKey) {
    string encryptedMessage;
    int keyIndex = 0;
    for (char c : message) {
        if (keyIndex >= publicKey.size()) {
            keyIndex = 0;
        }
        encryptedMessage += (c ^ publicKey[keyIndex].to_ulong());
        keyIndex++;
    }
    return encryptedMessage;
}

string decryptMessage(const string& encryptedMessage, const vector<bitset<100>>& publicKey) {
    string decryptedMessage;
    int keyIndex = 0;
    for (char c : encryptedMessage) {
        if (keyIndex >= publicKey.size()) {
            keyIndex = 0; 
        }
        decryptedMessage += (c ^ publicKey[keyIndex].to_ulong());
        keyIndex++;
    }
    return decryptedMessage;
}



