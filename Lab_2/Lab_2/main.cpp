#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define BASE64_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

std::unordered_map<char, int> create_base64_mapping() {
    std::unordered_map<char, int> base64_mapping;
    std::string base64_chars = BASE64_CHARS;
    int value = 0;
    for (char c : base64_chars) {
        base64_mapping[c] = value++;
    }
    return base64_mapping;
}

int get_base64_value(char c, const std::unordered_map<char, int>& base64_mapping) {
    auto it = base64_mapping.find(c);
    if (it != base64_mapping.end()) {
        return it->second;
    }
    return -1;
}

std::string generate_text(int length) {
    std::string text;
    int chars_count = std::strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    std::srand(std::time(0));

    for (int i = 0; i < length; ++i) {
        int index = std::rand() % chars_count;
        text += BASE64_CHARS[index];
    }
    return text;
}

std::string base64_encode(const std::string& s) {
    const std::string base64_chars = BASE64_CHARS;

    std::string result;
    int pad_count = 0;
    int c = s.length() % 3;

    std::string modified_s = s;
    if (c > 0) {
        pad_count = 3 - c;
        modified_s.resize(modified_s.length() + pad_count, '\0');
    }

    for (size_t i = 0; i < modified_s.length(); i += 3) {
        if (i > 0 && (i / 3 * 4) % 76 == 0) {
            result += "\r\n";
        }

        int n = (modified_s[i] << 16) + (modified_s[i + 1] << 8) + modified_s[i + 2];

        int n0 = (n >> 18) & 63;
        int n1 = (n >> 12) & 63;
        int n2 = (n >> 6) & 63;
        int n3 = n & 63;

        result += base64_chars[n0];
        result += base64_chars[n1];
        if (i + 2 < s.length()) {
            result += base64_chars[n2];
            result += base64_chars[n3];
        }
    }

    result.append(pad_count, '=');

    return result;
}

std::string xor_buffers(const std::string& buffer_a, const std::string& buffer_b) {
    size_t min_length = std::min(buffer_a.length(), buffer_b.length());

    std::string result;
    result.reserve(min_length);

    for (size_t i = 0; i < min_length; ++i) {
        result.push_back(buffer_a[i] ^ buffer_b[i]);
    }

    return result;
}

void pad_buffer_with_zeros(std::string& buffer, size_t length) {
    while (buffer.length() < length) {
        buffer.push_back(0);
    }
}

int main() {
    std::unordered_map<char, int> base64_mapping = create_base64_mapping();

    int text_length = 300;
    std::string generated_text = generate_text(text_length);
    std::cout << "Generated Text: " << generated_text << std::endl;

    std::string text = generated_text;
    std::string encoded_text = base64_encode(text);
    std::cout << "\nEncoded: " << encoded_text << std::endl;

    std::string surname_ascii = "Kryshtal";
    std::string name_ascii = "Maksim";

    size_t max_length = std::max(surname_ascii.length(), name_ascii.length());
    pad_buffer_with_zeros(surname_ascii, max_length);
    pad_buffer_with_zeros(name_ascii, max_length);

    std::string result_ascii = xor_buffers(surname_ascii, name_ascii);
    std::cout << "Result (ASCII): " << result_ascii << std::endl;

    std::string surname_base64 = base64_encode(surname_ascii);
    std::string name_base64 = base64_encode(name_ascii);

    max_length = std::max(surname_base64.length(), name_base64.length());
    pad_buffer_with_zeros(surname_base64, max_length);
    pad_buffer_with_zeros(name_base64, max_length);

    std::string result_base64 = xor_buffers(surname_base64, name_base64);
    std::cout << "Result (Base64): " << result_base64 << std::endl;

    return 0;
}
