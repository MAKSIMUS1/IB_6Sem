#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <map>

using namespace std;
using namespace std::chrono;

class Encryption {
private:
    vector<int> key;
    vector<wchar_t> german_alphabet = {
        L'A', L'Ä', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I',
        L'J', L'K', L'L', L'M', L'N', L'O', L'Ö', L'P', L'Q', L'R',
        L'S', L'ß', L'T', L'U', L'Ü', L'V', L'W', L'X', L'Y', L'Z'
    };

public:
    void SetKey(string _key) {
        for (char c : _key)
            key.push_back(c - '0');
    }

    string Encrypt(string input) {
        auto start = high_resolution_clock::now(); // Начало отсчета времени

        for (int i = 0; i < input.length() % key.size(); i++)
            input += input[i];

        vector<vector<char>> table((input.length() + key.size() - 1) / key.size(), vector<char>(key.size()));

        int index = 0;
        for (int i = 0; i < table.size(); i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < key.size(); j++) {
                    if (index < input.length())
                        table[i][j] = input[index++];
                    else
                        table[i][j] = ' ';
                }
            }
            else {
                for (int j = key.size() - 1; j >= 0; j--) {
                    if (index < input.length())
                        table[i][j] = input[index++];
                    else
                        table[i][j] = ' ';
                }
            }
        }

        string result = "";
        for (int j = 0; j < key.size(); j++) {
            for (int i = 0; i < table.size(); i++) {
                result += table[i][key[j] - 1];
            }
        }

        auto stop = high_resolution_clock::now(); // Конец отсчета времени
        auto duration = duration_cast<microseconds>(stop - start); // Вычисление продолжительности

        cout << "Encryption Time: " << duration.count() << " microseconds" << endl;

        return result;
    }

    string Decrypt(string input) {
        auto start = high_resolution_clock::now(); // Начало отсчета времени

        vector<vector<char>> table((input.length() + key.size() - 1) / key.size(), vector<char>(key.size()));

        int index = 0;
        for (int j = 0; j < key.size(); j++) {
            for (int i = 0; i < table.size(); i++) {
                if (index < input.length())
                    table[i][key[j] - 1] = input[index++];
                else
                    table[i][key[j] - 1] = ' ';
            }
        }

        string result = "";
        for (int i = 0; i < table.size(); i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < key.size(); j++) {
                    result += table[i][j];
                }
            }
            else {
                for (int j = key.size() - 1; j >= 0; j--) {
                    result += table[i][j];
                }
            }
        }

        auto stop = high_resolution_clock::now(); // Конец отсчета времени
        auto duration = duration_cast<microseconds>(stop - start); // Вычисление продолжительности

        cout << "Decryption Time: " << duration.count() << " microseconds" << endl;

        return result.substr(0, input.length());
    }
};

class MultiplePermutationEncryption {
private:
    string key1 = "MAKSIM";
    string key2 = "KRISHTAL";

public:
    string Encrypt(string input) {
        auto start = high_resolution_clock::now(); // Начало отсчета времени

        string encrypted_message = input;
        for (char c : key1) {
            string::size_type pos = encrypted_message.find(c);
            if (pos != string::npos) {
                rotate(encrypted_message.begin(), encrypted_message.begin() + pos, encrypted_message.end());
            }
        }

        vector<string> blocks;
        for (size_t i = 0; i < encrypted_message.length(); i += key2.length()) {
            blocks.push_back(encrypted_message.substr(i, key2.length()));
        }

        sort(blocks.begin(), blocks.end(), [&](const string& a, const string& b) {
            for (char c : key2) {
                if (a.find(c) < b.find(c)) return true;
                else if (a.find(c) > b.find(c)) return false;
            }
            return false;
            });

        encrypted_message = "";
        for (const string& block : blocks) {
            encrypted_message += block;
        }

        auto stop = high_resolution_clock::now(); // Конец отсчета времени
        auto duration = duration_cast<microseconds>(stop - start); // Вычисление продолжительности

        cout << "Encryption Time: " << duration.count() << " microseconds" << endl;

        return encrypted_message;
    }

    string Decrypt(string input) {
        auto start = high_resolution_clock::now(); // Начало отсчета времени

        vector<string> blocks;
        for (size_t i = 0; i < input.length(); i += key2.length()) {
            blocks.push_back(input.substr(i, key2.length()));
        }
        sort(blocks.begin(), blocks.end(), [&](const string& a, const string& b) {
            for (char c : key2) {
                if (a.find(c) < b.find(c)) return true;
                else if (a.find(c) > b.find(c)) return false;
            }
            return false;
            });

        string decrypted_message = "";
        for (const string& block : blocks) {
            decrypted_message += block;
        }

        for (int i = key1.length() - 1; i >= 0; --i) {
            string::size_type pos = decrypted_message.find(key1[i]);
            if (pos != string::npos) {
                rotate(decrypted_message.begin(), decrypted_message.begin() + pos, decrypted_message.end());
            }
        }

        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start);

        cout << "Decryption Time: " << duration.count() << " microseconds" << endl;

        return decrypted_message;
    }
};

std::map<wchar_t, int> count_characters_from_file(const std::string& filename) {
    vector<wchar_t> german_alphabet = {
        L'A', L'Ä', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I',
        L'J', L'K', L'L', L'M', L'N', L'O', L'Ö', L'P', L'Q', L'R',
        L'S', L'ß', L'T', L'U', L'Ü', L'V', L'W', L'X', L'Y', L'Z'
    };

    std::map<wchar_t, int> char_counts;

    std::wifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return char_counts;
    }

    wchar_t c;
    while (file.get(c)) {
        c = towupper(c);
        if (find(german_alphabet.begin(), german_alphabet.end(), c) != german_alphabet.end()) {
            ++char_counts[c];
        }
    }

    return char_counts;
}

int main() {
    ifstream input_file("input.txt");
    if (!input_file) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    ofstream output_file("output.txt");
    if (!output_file) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    string message((istreambuf_iterator<char>(input_file)), (istreambuf_iterator<char>()));
    input_file.close();

    Encryption encryption;
    string key = "3142";
    encryption.SetKey(key);

    string encrypted_message = encryption.Encrypt(message);
    string decrypted_message = encryption.Decrypt(encrypted_message);

    output_file << "Encrypted Message: " << encrypted_message << endl;
    output_file << "Decrypted Message: " << decrypted_message << endl;
    output_file.close();

    cout << "Message encrypted and saved to output.txt" << endl;

    // Множественная перестановка
    ifstream input_file_multiple("input.txt");
    if (!input_file_multiple) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    string message_multiple((istreambuf_iterator<char>(input_file_multiple)), (istreambuf_iterator<char>()));
    input_file_multiple.close();

    MultiplePermutationEncryption encryption_multiple;

    string encrypted_message_multiple = encryption_multiple.Encrypt(message_multiple);
    string decrypted_message_multiple = encryption_multiple.Decrypt(encrypted_message_multiple);

    ofstream output_file_multiple("output_multiple.txt");
    if (!output_file_multiple) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    output_file_multiple << "Encrypted Message: " << encrypted_message_multiple << endl;
    output_file_multiple << "Decrypted Message: " << decrypted_message_multiple << endl;
    output_file_multiple.close();

    cout << "Message encrypted and saved to output_multiple.txt" << endl;

    std::map<wchar_t, int> char_counts = count_characters_from_file("input.txt");
    std::cout << "Original Text Character Frequencies:" << std::endl;
    for (const auto& pair : char_counts) {
        std::wcout << (int)pair.first << ": " << pair.second << std::endl;
    }
    std::map<wchar_t, int> char_counts_output = count_characters_from_file("output.txt");
    std::cout << "Output Text Character Frequencies:" << std::endl;
    for (const auto& pair : char_counts_output) {
        std::wcout << (int)pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
