#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <codecvt>
#include <vector>
#include <locale>
#include <cmath>

double Entropy(std::vector<std::wstring> alphabet, std::string filename);
double BinaryEntropy(const std::string& filename);

double InformationContent(const std::string& text, double entropy) {
    return text.length() * entropy;
}

double ASCIIEntropy() {
    int alphabet_size = 128;
    double probability = 1.0 / alphabet_size;
    return -probability * log2(probability) * alphabet_size;
}

int main() {
    system("chcp 65001");
    std::vector<std::wstring> croatian_alphabet = {
        L"A", L"B", L"C", L"Č", L"Ć", L"D",
        L"Ð", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L",
        L"M", L"N", L"O", L"P", L"R", L"S", L"Š", L"T",
        L"U", L"V", L"Z", L"Ž"
    };
    std::vector<std::wstring> uzbek_alphabet = {
        L"А", L"Б",	L"В",	L"Г",	L"Д",	L"Е",	L"Ё",	L"Ж",	L"З",	L"И",
        L"Й", L"К",	L"Л",	L"М",	L"Н",	L"О",	L"П",	L"Р",	L"С",	L"Т",
        L"У", L"Ф",	L"Х",	L"Ц",	L"Ч",	L"Ш",	L"Ъ",	L"Ь",	L"Э",	L"Ю",
        L"Я", L"Ў",	L"Қ",	L"Ғ",	L"Ҳ",	L"ʼ"
    };

    std::vector<std::wstring> latin_alphabet = {
    L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J",
    L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T",
    L"U", L"V", L"W", L"X", L"Y", L"Z",
    };

    std::vector<std::wstring> base64_alphabet = {
    L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J",
    L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T",
    L"U", L"V", L"W", L"X", L"Y", L"Z", 
    L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"+", L"/"
    };

    //double croatian_entropy = Entropy(croatian_alphabet, "croatia.txt");
    //double uzbek_entropy = Entropy(uzbek_alphabet, "uzbek.txt");
    //double binary_entropy = BinaryEntropy("binary_data.bin");
    //double ascii_entropy = ASCIIEntropy();

    //std::wcout << L"Entropy[Latin]: " << croatian_entropy << std::endl;
    //std::wcout << L"Entropy[Kiril]: " << uzbek_entropy << std::endl;
    //std::cout << "Entropy of binary alphabet: " << binary_entropy << std::endl;

    //std::string croatian_FIO = "Kristal Maksim Olegović";
    //std::string uzbek_FIO = "Криштал Максим Олегович";
    //std::string ascii_FIO = "Krishtal Maxim Olegovich";

    //double croatian_info = InformationContent(croatian_FIO, croatian_entropy);
    //double uzbek_info = InformationContent(uzbek_FIO, uzbek_entropy); 
    //double ascii_info = ascii_FIO.length() * 8 * binary_entropy;


    //std::cout << "Information content for Croatian name: " << croatian_info << " bits" << std::endl;
    //std::cout << "Information content for Uzbek name: " << uzbek_info << " bits" << std::endl;
    //std::cout << "Information content for ASCII name: " << ascii_info << " bits" << std::endl;

    //double p_error[] = { 0.1, 0.5 };
    //for (double p : p_error) {
    //    double error_info = -p*log2(p)-(1-p)*log2(1-p);
    //    std::cout << "Error probability " << p * 100 << "%: " << error_info << " bits" << std::endl;
    //}

    //std::cout << "Error probability 100%: " << 0 << " bits" << std::endl;

    //lab2 base64
    std::cout << "\n\n-----------------------|||||||-----------------------\n" << std::endl;
    double in_base64 = Entropy(latin_alphabet, "IN_base64.txt");
    std::cout << "\n\n" << std::endl;
    double out_base64 = Entropy(base64_alphabet, "OUT_base64.txt");
    std::wcout << L"Entropy Shenon[ASCII  Text]: " << in_base64 << std::endl;
    std::wcout << L"Entropy Shenon[Base64 Text]: " << out_base64 << std::endl;

    std::wcout << L"Entropy Hartli[ASCII  Text]: " << log2(latin_alphabet.size()) << std::endl;
    std::wcout << L"Entropy Hartli[Base64 Text]: " << log2(64) << std::endl;

    std::wcout << L"R(a): " << ((log2(latin_alphabet.size()) - (in_base64)) / log2(latin_alphabet.size())) * 100 << L"%" << std::endl;
    std::wcout << L"R(b): " << ((log2(64) - (out_base64)) / log2(64)) * 100 << L"%" << std::endl;
    return 0;
}


double Entropy(std::vector<std::wstring> alphabet, std::string filename) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::ifstream inputFile(filename);
    std::u32string croatia_text;

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            croatia_text += converter.from_bytes(line);
            croatia_text += U'\n';
        }
        inputFile.close();
    }
    else {
        std::cerr << "Unable to open file!" << std::endl;
        return 1;
    }

    std::map<wchar_t, int> letterCount;

    for (const auto& letter : alphabet) {
        for (wchar_t c : letter) {
            letterCount[c] = 0;
        }
    }

    int totalLetters = 0;

    for (wchar_t c : croatia_text) {
        wchar_t upper_c = std::toupper(static_cast<wchar_t>(c), std::locale());
        if (letterCount.find(upper_c) != letterCount.end()) {
            letterCount[upper_c]++;
            totalLetters++;
        }
    }

    short g = 1;
    for (const auto& pair : letterCount) {
        std::wcout << g << L". \t" << L"[" << pair.first << L"]: " << pair.second << std::endl;
        g++;
    }

    double entropy = 0.0;
    for (const auto& pair : letterCount) {
        double probability = static_cast<double>(pair.second) / totalLetters;
        if (probability != 0) {
            entropy -= probability * log2(probability);
        }
    }

    return  entropy;
}

double BinaryEntropy(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return -1.0;
    }

    int count0 = 0;
    int count1 = 0;
    char c;
    while (file.get(c)) {
        if (c == '0') {
            count0++;
        }
        else if (c == '1') {
            count1++;
        }
    }
    file.close();

    int totalSymbols = count0 + count1;

    double p0 = static_cast<double>(count0) / totalSymbols;
    double p1 = static_cast<double>(count1) / totalSymbols;

    double entropy = -(p0 * log2(p0) + p1 * log2(p1));

    return entropy;
}