#include <iostream>
#include <bitset>
#include <cmath>
#include <random>

using namespace std;

const int N = 256;

bitset<N> generateBBS(int n, unsigned long long p, unsigned long long q) {
    unsigned long long nValue = p * q;
    unsigned long long s = static_cast<unsigned long long>(pow(rand() % nValue, 2)) % nValue;
    bitset<N> pseudorandomSequence;

    for (int i = 0; i < n; ++i) {
        s = (s * s) % nValue;
        pseudorandomSequence[i] = s & 1;
    }

    return pseudorandomSequence;
}

int main() {
    srand(time(nullptr));
    unsigned long long p = 607;
    unsigned long long q = 751;
    bitset<N> pseudorandomSequence = generateBBS(N, p, q);

    cout << "Pseudorandom Sequence: " << pseudorandomSequence << endl;

    return 0;
}
