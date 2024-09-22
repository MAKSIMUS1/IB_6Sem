#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <random>
#include <functional>

using namespace std;
using namespace std::chrono;

// ��������� ��������� �����
random_device rd;
mt19937 gen(rd());

// ������� ��� ���������� ���
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// ������� ��� ��������� ���������� �������� �����
unsigned long long generatePrime(unsigned long long minVal, unsigned long long maxVal) {
    uniform_int_distribution<unsigned long long> dist(minVal, maxVal);
    unsigned long long p = dist(gen);
    while (true) {
        bool isPrime = true;
        for (unsigned long long i = 2; i <= sqrt(p); ++i) {
            if (p % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) return p;
        p = dist(gen);
    }
}

// ������� ��� ���������� ����������� ���������� � �������
unsigned long long modPow(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// ��������� ������ RSA
void generateRSAKeys(unsigned long long& publicKey, unsigned long long& privateKey, unsigned long long& modulus) {
    unsigned long long p = generatePrime(10000, 50000);
    unsigned long long q = generatePrime(10000, 50000);
    modulus = p * q;
    unsigned long long phi = (p - 1) * (q - 1);
    publicKey = generatePrime(2, phi - 1);
    while (gcd(publicKey, phi) != 1) {
        publicKey = generatePrime(2, phi - 1);
    }
    unsigned long long a = publicKey;
    unsigned long long b = phi;
    unsigned long long x0 = 0, x1 = 1, y0 = 1, y1 = 0, t;
    while (b > 0) {
        q = a / b;
        t = b;
        b = a % b;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
        t = y0;
        y0 = y1 - q * y0;
        y1 = t;
    }
    privateKey = (x1 + phi) % phi;
}

// ������� ���������� RSA
string encryptRSA(const string& plaintext, unsigned long long publicKey, unsigned long long modulus) {
    string ciphertext = "";
    for (char c : plaintext) {
        unsigned long long charCode = c;
        unsigned long long encryptedCharCode = modPow(charCode, publicKey, modulus);
        ciphertext += to_string(encryptedCharCode) + " ";
    }
    return ciphertext;
}

// ������� ������������� RSA
string decryptRSA(const string& ciphertext, unsigned long long privateKey, unsigned long long modulus) {
    string plaintext = "";
    size_t pos = 0;
    string ciphertextCopy = ciphertext; // ������� ����� ������ ciphertext
    while ((pos = ciphertextCopy.find(' ')) != string::npos) {
        string token = ciphertextCopy.substr(0, pos);
        ciphertextCopy = ciphertextCopy.substr(pos + 1);
        unsigned long long encryptedCharCode = stoull(token);
        unsigned long long decryptedCharCode = modPow(encryptedCharCode, privateKey, modulus);
        plaintext += static_cast<char>(decryptedCharCode);
    }
    return plaintext;
}



// ��������� ������ ���-������
void generateElGamalKeys(unsigned long long& publicKey, unsigned long long& privateKey, unsigned long long& p) {
    p = generatePrime(5000, 10000);
    unsigned long long g = generatePrime(2, p - 1);
    privateKey = generatePrime(2, p - 1);
    publicKey = modPow(g, privateKey, p);
}

// ������� ���������� ���-������
pair<unsigned long long, unsigned long long> encryptElGamal(unsigned long long plaintext, unsigned long long publicKey, unsigned long long p) {
    unsigned long long k = generatePrime(2, p - 1);
    unsigned long long c1 = modPow(publicKey, k, p);
    unsigned long long c2 = (plaintext * modPow(publicKey, k, p)) % p;
    return make_pair(c1, c2);
}


// ������� ������������� ���-������
unsigned long long decryptElGamal(pair<unsigned long long, unsigned long long> ciphertext, unsigned long long privateKey, unsigned long long p) {
    unsigned long long c1 = ciphertext.first;
    unsigned long long c2 = ciphertext.second;
    unsigned long long denominator = modPow(c1, privateKey, p);
    unsigned long long decryptedPlaintext = (c2 * denominator) % p;
    return decryptedPlaintext;
}

int main() {
    // ��������� ������ RSA
    unsigned long long rsaPublicKey, rsaPrivateKey, rsaModulus;
    generateRSAKeys(rsaPublicKey, rsaPrivateKey, rsaModulus);

    // ��������� ������ ���-������
    unsigned long long elGamalPublicKey, elGamalPrivateKey, elGamalP;
    generateElGamalKeys(elGamalPublicKey, elGamalPrivateKey, elGamalP);

    // �������� ����� ��� ����������
    string plaintext = "KRYSHTAL MAKSIM OLEGOVICH";

    // ��������� ���� ��������� ������
    unsigned long long plaintextHash = hash<string>{}(plaintext);

    // ���������� RSA
    string encryptedRSA;
    auto startRSAEncrypt = high_resolution_clock::now();
    encryptedRSA = encryptRSA(plaintext, rsaPublicKey, rsaModulus);
    auto endRSAEncrypt = high_resolution_clock::now();
    auto durationRSAEncrypt = duration_cast<microseconds>(endRSAEncrypt - startRSAEncrypt);

    // ���������� ���-������
    pair<unsigned long long, unsigned long long> encryptedElGamal;
    auto startElGamalEncrypt = high_resolution_clock::now();
    encryptedElGamal = encryptElGamal(plaintextHash, elGamalPublicKey, elGamalP);
    auto endElGamalEncrypt = high_resolution_clock::now();
    auto durationElGamalEncrypt = duration_cast<microseconds>(endElGamalEncrypt - startElGamalEncrypt);

    // ������ ��������� ������� �������������
    double ratioRSAToPlaintext = static_cast<double>(encryptedRSA.size()) / plaintext.size();
    double ratioElGamalToPlaintext = static_cast<double>(to_string(encryptedElGamal.first).size() + to_string(encryptedElGamal.second).size()) / plaintext.size();

    // ����� �����������
    cout << "Original Text: " << plaintext << endl;
    cout << "RSA Encrypted Text: " << encryptedRSA << endl;
    cout << "RSA Decrypted Text: " << decryptRSA(encryptedRSA, rsaPrivateKey, rsaModulus) << endl;
    cout << "ElGamal Encrypted Text: (" << encryptedElGamal.first << ", " << encryptedElGamal.second << ")" << endl;
    cout << "ElGamal Decrypted Text: " << decryptElGamal(encryptedElGamal, elGamalPrivateKey, elGamalP) << endl;

    // ������ ������� ���������� ��������
    cout << "RSA Encryption Time: " << durationRSAEncrypt.count() << " microseconds" << endl;
    cout << "ElGamal Encryption Time: " << durationElGamalEncrypt.count() << " microseconds" << endl;

    // ������ �������������� ��������� ������� �������������
    cout << "Ratio of RSA ciphertext size to plaintext size: " << ratioRSAToPlaintext << endl;
    cout << "Ratio of ElGamal ciphertext size to plaintext size: " << ratioElGamalToPlaintext << endl;

    return 0;
}

