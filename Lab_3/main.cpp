#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int gcd(int a, int b, int c) {
    return gcd(gcd(a, b), c);
}

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

std::vector<int> find_primes(int start, int end) {
    std::vector<int> primes;
    for (int i = start; i <= end; ++i) {
        if (is_prime(i)) {
            primes.push_back(i);
        }
    }
    return primes;
}

std::string int_to_string(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

bool is_concatenated_number_prime(int m, int n) {
    std::string concatenated_str = int_to_string(m) + int_to_string(n);
    int concatenated_num = std::stoi(concatenated_str);
    return is_prime(concatenated_num);
}

std::vector<int> prime_factors(int num) {
    std::vector<int> factors;
    for (int i = 2; i * i <= num; ++i) {
        while (num % i == 0) {
            factors.push_back(i);
            num /= i;
        }
    }
    if (num > 1) {
        factors.push_back(num);
    }
    return factors;
}

void print_canonical_form(int num) {
    std::vector<int> factors = prime_factors(num);
    std::cout << num << " = ";
    if (factors.empty()) {
        std::cout << "1" << std::endl;
        return;
    }
    std::cout << factors[0];
    int count = 1;
    for (size_t i = 1; i < factors.size(); ++i) {
        if (factors[i] == factors[i - 1]) {
            ++count;
        }
        else {
            if (count > 1) {
                std::cout << "^" << count;
                count = 1;
            }
            std::cout << " x " << factors[i];
        }
    }
    if (count > 1) {
        std::cout << "^" << count;
    }
    std::cout << std::endl;
}

int main() {
    setlocale(LC_ALL, "ru_RU");

    int m = 399, n = 433, start = 2;
    //int m = 400, n = 600;

    std::vector<int> primes = find_primes(start, n);
    std::cout << "Простые числа в диапазоне от " << start << " до " << n << ": ";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;
    int primeCount = primes.size();
    std::cout << "Количество простых чисел в интервале [" << start << ", " << n << "]: " << primeCount << std::endl;

    double expectedCount = (n / std::log(n));
    std::cout << "n/ln(n): " << expectedCount << " [n = " << n << "]" << std::endl;


    primes = find_primes(m, n);
    std::cout << "Простые числа в диапазоне от " << m << " до " << n << ": ";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;
    primeCount = primes.size();
    std::cout << "Количество простых чисел в интервале [" << m << ", " << n << "]: " << primeCount << std::endl;

    std::cout << "Каноническая форма числа m = " << m << ":" << std::endl;
    print_canonical_form(m);

    std::cout << "Каноническая форма числа n = " << n << ":" << std::endl;
    print_canonical_form(n);

    print_canonical_form(18);
    print_canonical_form(2770);
    print_canonical_form(3780);
    print_canonical_form(6224);
    bool concatenated_prime = is_concatenated_number_prime(m, n);
    std::cout << "Число, состоящее из конкатенации цифр " << m << " и " << n << ", является простым: " << std::boolalpha << concatenated_prime << std::endl;

    std::cout << "НОД чисел " << n << " и " << m << ": " << gcd(n, m) << std::endl;
    int g = 1000;
    std::cout << "НОД чисел " << n << ", " << m << " и " << g << ": " << gcd(n, m, g) << std::endl;


    return 0;
}