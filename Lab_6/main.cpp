#include <iostream>
#include <vector>
#include <algorithm>

class Rotor {
private:
    std::vector<char> mapping;
    int position;
public:
    Rotor(std::vector<char> mapping) : mapping(mapping), position(0) {}

    void setPosition(int pos) {
        position = pos;
    }

    char encrypt(char input) {
        char mappedChar = mapping[(input - 'A' + position) % 26];
        return mappedChar;
    }

    void rotate() {
        position = (position + 1) % 26;
    }

    int getPosition() const {
        return position;
    }
};

class Reflector {
private:
    std::vector<std::pair<char, char>> pairs;
public:
    Reflector(std::vector<std::pair<char, char>> pairs) : pairs(pairs) {}

    char reflect(char input) {
        for (const auto& pair : pairs) {
            if (pair.first == input)
                return pair.second;
            else if (pair.second == input)
                return pair.first;
        }
        return input;
    }
};

class EnigmaMachine {
private:
    Rotor leftRotor;
    Rotor middleRotor;
    Rotor rightRotor;
    Reflector reflector;
    std::string initialRotorPositions;
public:
    EnigmaMachine(Rotor left, Rotor middle, Rotor right, Reflector reflector)
        : leftRotor(left), middleRotor(middle), rightRotor(right), reflector(reflector) {
        initialRotorPositions = {
            static_cast<char>('A' + leftRotor.getPosition()),
            static_cast<char>('A' + middleRotor.getPosition()),
            static_cast<char>('A' + rightRotor.getPosition())
        };
    }

    char encrypt(char input) {
        std::string savedRotorPositions = initialRotorPositions;

        input = rightRotor.encrypt(input);
        input = middleRotor.encrypt(input);
        input = leftRotor.encrypt(input);

        input = reflector.reflect(input);

        input = leftRotor.encrypt(input);
        input = middleRotor.encrypt(input);
        input = rightRotor.encrypt(input);

        rightRotor.rotate();
        if (rightRotor.getPosition() == 0) {
            middleRotor.rotate();
        }
        leftRotor.rotate();

        return input;
    }

    char decrypt(char input) {
        return encrypt(input);
    }
};

int main() {
    Rotor rotorL({ 'F', 'K', 'Q', 'H', 'T', 'L', 'X', 'O', 'C', 'B', 'J', 'S', 'P', 'D', 'Z', 'R', 'A', 'M', 'E', 'W', 'N', 'I', 'U', 'Y', 'G', 'V' });
    Rotor rotorM({ 'A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W', 'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E' });
    Rotor rotorR({ 'E', 'S', 'O', 'V', 'P', 'Z', 'J', 'A', 'Y', 'Q', 'U', 'I', 'R', 'H', 'X', 'L', 'N', 'F', 'T', 'G', 'K', 'D', 'C', 'M', 'W', 'B' });

    rotorL.setPosition(0);
    rotorM.setPosition(0);
    rotorR.setPosition(0);

    Reflector reflector({ {'A', 'Y'}, {'B', 'R'}, {'C', 'U'}, {'D', 'H'}, {'E', 'Q'}, {'F', 'S'}, {'G', 'L'}, {'I', 'P'}, {'J', 'X'}, {'K', 'N'}, {'M', 'O'}, {'T', 'Z'}, {'V', 'W'} });

    EnigmaMachine machine(rotorL, rotorM, rotorR, reflector);

    std::string message = "AA";
    std::string encryptedMessage;

    for (char c : message) {
        if (isalpha(c)) {
            encryptedMessage += machine.encrypt(toupper(c));
        }
        else {
            encryptedMessage += c;
        }
    }

    std::cout << "Message: " << message << std::endl;
    std::cout << "Encrypted message: " << encryptedMessage << std::endl;

    std::string decryptedMessage;
    for (char c : encryptedMessage) {
        if (isalpha(c)) {
            decryptedMessage += machine.decrypt(c);
        }
        else {
            decryptedMessage += c;
        }
    }

    std::cout << "Decrypted message: " << message << std::endl;

    return 0;
}
