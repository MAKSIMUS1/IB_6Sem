#include <iostream>
#include <fstream>
#include <locale>
#include <vector>
#include <wchar.h>
#include <cwctype>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include <chrono>


std::vector<wchar_t> german_alphabet = {
    L'A', L'Ä', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I',
    L'J', L'K', L'L', L'M', L'N', L'O', L'Ö', L'P', L'Q', L'R',
    L'S', L'ß', L'T', L'U', L'Ü', L'V', L'W', L'X', L'Y', L'Z'
};

void encrypt_file(const std::string& input_filename, const std::string& output_filename, int k) {
    std::wifstream inputFile(input_filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    std::wofstream outputFile(output_filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        inputFile.close();
        return;
    }

    int N = german_alphabet.size();
    std::wstring line;
    while (std::getline(inputFile, line)) {
        std::transform(line.begin(), line.end(), line.begin(), std::towupper);
        for (wchar_t& c : line) {
            auto it = std::find(german_alphabet.begin(), german_alphabet.end(), c);
            if (it != german_alphabet.end()) {
                size_t index = (std::distance(german_alphabet.begin(), it) + k) % N;
                c = german_alphabet[index];
            }
        }
        outputFile << line << std::endl;
    }

    inputFile.close();
    outputFile.close();
}

void decrypt_file(const std::string& input_filename, const std::string& output_filename, int k) {
    std::wifstream inputFile(input_filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    std::wofstream outputFile(output_filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        inputFile.close();
        return;
    }

    int N = german_alphabet.size();
    std::wstring line;
    while (std::getline(inputFile, line)) {
        std::transform(line.begin(), line.end(), line.begin(), std::towupper);
        for (wchar_t& c : line) {
            auto it = std::find(german_alphabet.begin(), german_alphabet.end(), c);
            if (it != german_alphabet.end()) {
                size_t index = (std::distance(german_alphabet.begin(), it) - k + N) % N;
                c = german_alphabet[index];
            }
        }
        outputFile << line << std::endl;
    }

    inputFile.close();
    outputFile.close();
}

std::vector<std::vector<wchar_t>> generate_trisemus_table(const std::wstring& keyword) {
    std::vector<std::vector<wchar_t>> table(5, std::vector<wchar_t>(6, L' '));

    for (size_t i = 0; i < keyword.size(); ++i) {
        table[0][i] = keyword[i];
    }

    std::set<wchar_t> used_chars(keyword.begin(), keyword.end());

    size_t index = 0;
    auto it = german_alphabet.begin();

    while (index < table[1].size() && it != german_alphabet.end()) {
        wchar_t c = *it;
        if (used_chars.find(c) == used_chars.end()) {
            table[1][index++] = c;
            used_chars.insert(c);
        }
        ++it; 
    }

    index = 0;
    for (size_t i = 2; i < 5; ++i) {
        for (size_t j = 0; j < 6; ++j) {
            wchar_t next_char = german_alphabet[index++];
            while (used_chars.find(next_char) != used_chars.end()) {
                next_char = german_alphabet[index++];
            }
            table[i][j] = next_char;
        }
    }

    return table;
}


void print_trisemus_table(const std::vector<std::vector<wchar_t>>& table) {
    for (const auto& row : table) {
        for (wchar_t symbol : row) {
            std::wcout << symbol << " ";
        }
        std::wcout << std::endl;
    }
}

std::wstring encrypt_decrypt_with_trisemus(const std::wstring& text, const std::vector<std::vector<wchar_t>>& table, bool encrypt) {
    std::wstring result;

    for (wchar_t c : text) {
        c = std::towupper(c);

        if (std::find(german_alphabet.begin(), german_alphabet.end(), c) != german_alphabet.end()) {
            for (size_t row = 0; row < table.size(); ++row) {
                for (size_t col = 0; col < table[row].size(); ++col) {
                    if (c == table[row][col]) {
                        size_t next_row = (row + (encrypt ? 1 : table.size() - 1)) % table.size();
                        result += table[next_row][col];
                        break;
                    }
                }
            }
        }
        else {
            result += c;
        }
    }

    return result;
}

void process_file_with_trisemus(const std::string& input_filename, const std::string& output_filename, const std::vector<std::vector<wchar_t>>& table, bool encrypt) {
    std::wifstream inputFile(input_filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    std::wofstream outputFile(output_filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        inputFile.close();
        return;
    }

    wchar_t c;
    while (inputFile.get(c)) {
        outputFile << encrypt_decrypt_with_trisemus(std::wstring(1, c), table, encrypt);
    }

    inputFile.close();
    outputFile.close();
}

std::map<wchar_t, int> count_characters_from_file(const std::string& filename) {
    std::map<wchar_t, int> char_counts;

    std::wifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return char_counts;
    }

    wchar_t c;
    while (file.get(c)) {
        c = std::towupper(c);
        if (std::find(german_alphabet.begin(), german_alphabet.end(), c) != german_alphabet.end()) {
            ++char_counts[c];
        }
    }

    return char_counts;
}


int main() {
    std::locale::global(std::locale("de_DE.UTF-8"));

    std::string input_filename = "input.txt";
    std::string output_encrypt_filename = "encrypted_text.txt";
    std::string output_decrypt_filename = "decrypted_text.txt";
    std::string output_encrypt_trisemus_filename = "encrypted_text_trisemus.txt";
    std::string output_decrypt_trisemus_filename = "decrypted_text_trisemus.txt";
    int key = 7;

    auto start_encrypt = std::chrono::steady_clock::now();
    encrypt_file(input_filename, output_encrypt_filename, key); 
    auto end_encrypt = std::chrono::steady_clock::now();
    std::cout << "Encryption complete. Encrypted text written to " << output_encrypt_filename << std::endl;
    auto duration_encrypt = std::chrono::duration_cast<std::chrono::milliseconds>(end_encrypt - start_encrypt);
    std::cout << "Encryption Time: " << duration_encrypt.count() << " milliseconds" << std::endl;

    auto start_decrypt = std::chrono::steady_clock::now();
    decrypt_file(output_encrypt_filename, output_decrypt_filename, key);
    auto end_decrypt = std::chrono::steady_clock::now();
    std::cout << "Decryption complete. Decrypted text written to " << output_decrypt_filename << std::endl;
    auto duration_decrypt = std::chrono::duration_cast<std::chrono::milliseconds>(end_decrypt - start_decrypt);
    std::cout << "Decryption Time: " << duration_decrypt.count() << " milliseconds" << std::endl;

    
    std::wstring keyword = L"ENIGMA";
    std::vector<std::vector<wchar_t>> trisemus_table = generate_trisemus_table(keyword);
    print_trisemus_table(trisemus_table);


    auto start_encrypt_trisemus = std::chrono::steady_clock::now();
    process_file_with_trisemus(input_filename, output_encrypt_trisemus_filename, trisemus_table, true);
    auto end_encrypt_trisemus = std::chrono::steady_clock::now();
    std::cout << "Encryption[Trisemus] complete. Encrypted text written to " << output_encrypt_trisemus_filename << std::endl;
    auto duration_encrypt_trisemus = std::chrono::duration_cast<std::chrono::milliseconds>(end_encrypt_trisemus - start_encrypt_trisemus);
    std::cout << "Encryption[Trisemus] Time: " << duration_encrypt_trisemus.count() << " milliseconds" << std::endl;

    auto start_decrypt_trisemus = std::chrono::steady_clock::now();
    process_file_with_trisemus(output_encrypt_trisemus_filename, output_decrypt_trisemus_filename, trisemus_table, false);
    auto end_decrypt_trisemus = std::chrono::steady_clock::now();
    std::cout << "Decryption[Trisemus] complete. Decrypted text written to " << output_decrypt_trisemus_filename << std::endl;
    auto duration_decrypt_trisemus = std::chrono::duration_cast<std::chrono::milliseconds>(end_decrypt_trisemus - start_decrypt_trisemus);
    std::cout << "Decryption[Trisemus] Time: " << duration_decrypt_trisemus.count() << " milliseconds" << std::endl;

    std::map<wchar_t, int> original_char_counts = count_characters_from_file("decrypted_text.txt");
    std::map<wchar_t, int> encrypted_char_counts = count_characters_from_file("encrypted_text.txt");

    std::cout << "Original Text Character Frequencies:" << std::endl;
    for (const auto& pair : original_char_counts) {
        std::wcout << (int)pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nEncrypted Text Character Frequencies:" << std::endl;
    for (const auto& pair : encrypted_char_counts) {
        std::wcout << pair.first << ": " << pair.second << std::endl;
    }

    std::map<wchar_t, int> original_char_counts_trisemus = count_characters_from_file("decrypted_text_trisemus.txt");
    std::map<wchar_t, int> encrypted_char_counts_trisemus = count_characters_from_file("encrypted_text_trisemus.txt");

    std::cout << "Original[Trisemus] Text Character Frequencies:" << std::endl;
    for (const auto& pair : original_char_counts_trisemus) {
        std::wcout << (int)pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "\nEncrypted[Trisemus] Text Character Frequencies:" << std::endl;
    for (const auto& pair : encrypted_char_counts_trisemus) {
        std::wcout << pair.first << ": " << pair.second << std::endl;
    }
    return 0;
}
/*
E N I G M A
Ä B C D F H
J K L O Ö P
Q R S ß T U
Ü V W X Y Z
*/