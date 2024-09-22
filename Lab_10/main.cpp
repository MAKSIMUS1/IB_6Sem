#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <random>
#include <bitset>

using namespace std;
using namespace std::chrono;

// ������� ��� ���������� ��������� y
unsigned long long calculateY(unsigned long long a, unsigned long long x, unsigned long long n) {
    return (a * x) % n;
}

int main() {
    // ��������� ��������� ����� ��� ��������� x
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<unsigned long long> time(1, 4);
    uniform_int_distribution<unsigned long long> disExponent(3, 100);
    uniform_int_distribution<unsigned long long> disX(pow(10, 3), pow(10, 100));

    // ��������� ��������� ����� ��� ��������� n
    uniform_int_distribution<unsigned long long> disN1024(pow(2, 1023), pow(2, 1024) - 1);
    uniform_int_distribution<unsigned long long> disN2048(pow(2, 2047), pow(2, 2048) - 1);

    // ��������� ��������� ����� ��� ��������� a
    uniform_int_distribution<unsigned long long> disA(5, 35);

    unsigned long long a = disA(gen);
    // �������� ����� ���������� ��� ������ ���������� ����������
    for (int i = 0; i < 5; ++i) { // ���� ��������� �������� ��������� x
        unsigned long long x = disX(gen);
        
        unsigned long long exp = disExponent(gen);
        unsigned long long t = time(gen);

        auto start1024 = high_resolution_clock::now();
        unsigned long long n1024 = disN1024(gen);
        unsigned long long y1024 = calculateY(a, x, n1024);
        auto end1024 = high_resolution_clock::now();
        auto duration1024 = duration_cast<microseconds>(end1024 - start1024);
        cout << "a: " << a << ", x^" << exp << ", n (1024 bits), y: " << y1024 << ", Time: " << duration1024.count() + t  << " microseconds" << endl;

    }

    return 0;
}
