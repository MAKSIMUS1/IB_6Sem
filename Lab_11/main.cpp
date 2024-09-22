#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>
#include <random>

// ��������������� ������� ��� ������������ ������ ������
constexpr uint32_t ROTR(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

// ��������������� ������� ��� ������������ ������ �����
constexpr uint32_t ROTL(uint32_t x, uint32_t n) {
    return (x << n) | (x >> (32 - n));
}

// ��������������� ������� ��� ��������
constexpr uint32_t SIG0(uint32_t x) {
    return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}

// ��������������� ������� ��� ��������
constexpr uint32_t SIG1(uint32_t x) {
    return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}

// ��������������� ������� ��� �������
constexpr uint32_t EP0(uint32_t x) {
    return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
}

// ��������������� ������� ��� �������
constexpr uint32_t EP1(uint32_t x) {
    return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
}

// SHA-256 ������� �����������
std::string sha256(const std::string& message) {
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    const uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    std::string padded_message = message;
    padded_message += static_cast<char>(0x80); // ��������� ��� 1

    // ��������� ������� ����, ����� ����� ��������� ���� ������ 512 (448 + 64)
    while ((padded_message.size() * 8) % 512 != 448) {
        padded_message += static_cast<char>(0);
    }

    // ��������� ����� ��������� ��������� � ����� (64 ����)
    uint64_t message_length_bits = message.size() * 8;
    for (int i = 0; i < 8; ++i) {
        padded_message += static_cast<char>((message_length_bits >> (56 - i * 8)) & 0xFF);
    }

    // ��������� �� ����� �� 512 ���
    for (size_t i = 0; i < padded_message.size(); i += 64) {
        uint32_t w[64] = { 0 };

        // �������������� w �� ����� 512 ���
        for (int j = 0; j < 16; ++j) {
            w[j] = (padded_message[i + j * 4] << 24) |
                (padded_message[i + j * 4 + 1] << 16) |
                (padded_message[i + j * 4 + 2] << 8) |
                (padded_message[i + j * 4 + 3]);
        }

        // ��������� w �� 64 ���������
        for (int j = 16; j < 64; ++j) {
            w[j] = EP1(w[j - 2]) + w[j - 7] + EP0(w[j - 15]) + w[j - 16];
        }

        uint32_t a = h[0];
        uint32_t b = h[1];
        uint32_t c = h[2];
        uint32_t d = h[3];
        uint32_t e = h[4];
        uint32_t f = h[5];
        uint32_t g = h[6];
        uint32_t k = h[7];

        // �������� ����
        for (int j = 0; j < 64; ++j) {
            uint32_t t1 = h[7] + SIG1(e) + ((e & f) ^ (~e & g)) + k + w[j];
            uint32_t t2 = SIG0(a) + ((a & b) ^ (a & c) ^ (b & c));

            h[7] = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        // ��������� ���
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += k;
    }

    // �������� ��������� � ������
    std::string result;
    for (int i = 0; i < 8; ++i) {
        result += static_cast<char>((h[i] >> 24) & 0xFF);
        result += static_cast<char>((h[i] >> 16) & 0xFF);
        result += static_cast<char>((h[i] >> 8) & 0xFF);
        result += static_cast<char>((h[i]) & 0xFF);
    }

    return result;
}


// ��������� ��������� �����
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> distribution(1, 1024); // ���������� ��������� ��������� �� 1 �� 1024 ����

// ��������� ������� ����������
template <typename Func>
double measure_execution_time(Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

int main() {
    // ������ ������������� sha256 ��� ��������� ������
    std::string message1 = "Hello, world!";
    std::string hash1 = sha256(message1);
    std::cout << "Hash of 'Hello, world!': " << hash1 << std::endl;

    std::string message2 = "This is a test message.";
    std::string hash2 = sha256(message2);
    std::cout << "Hash of 'This is a test message.': " << hash2 << std::endl;

    std::string message3 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    std::string hash3 = sha256(message3);
    std::cout << "Hash of 'Lorem ipsum dolor sit amet, consectetur adipiscing elit.': " << hash3 << std::endl;

    const int num_iterations = 1000; // ���������� �������� ��� ���������� ������� ����������
    const int min_message_size = 128; // ����������� ������ ��������� � ������ (128 ���)
    const int max_message_size = 1024; // ������������ ������ ��������� � ������
    const int block_size = 128; // ������ ����� ��������� � ������ (128 ���)

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 255); // ��� ��������� ��������� ������

    for (int message_size = min_message_size; message_size <= max_message_size; message_size += block_size) {
        double total_time = 0.0;

        for (int i = 0; i < num_iterations; ++i) {
            // ���������� ��������� ��������� ��������� �������
            std::string message;
            for (int j = 0; j < message_size; ++j) {
                message += static_cast<char>(distribution(gen));
            }

            // �������� ����� ���������� ����������� ����� ���������
            double execution_time = measure_execution_time([&]() {
                sha256(message);
                });

            // ��������� ����� ���������� �� ���� ���������
            total_time += execution_time;
        }

        double average_time = total_time / num_iterations;
        std::cout << "Message size: " << message_size << " bytes, Average time: " << average_time << " seconds" << std::endl;
    }

    return 0;
}
