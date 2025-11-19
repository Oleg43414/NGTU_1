#ifndef MORSE_STANDALONE_H
#define MORSE_STANDALONE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class MorseCode {
private:
    map<char, string> morseMap;      // Убрал static
    map<string, char> reverseMap;    // Убрал static
    
    void initializeMaps();

public:
    MorseCode();
    string encodeChar(char c) const;
    string encodeString(const string& text) const;
    char decodeChar(const string& morse) const;
    string decodeString(const string& morse) const;
    bool isSupported(char c) const;
    void printSupportedChars() const;
    void runDemo() const;
};

// Структуры для бинарного кодирования
struct MorseEncodedResult {
    vector<unsigned char> binary_data;
    bool success;
};

struct MorseDecodedResult {
    string plaintext;
    bool success;
    string error_message;
};

struct MorseFileOperationResult {
    bool success;
    string message;
};

// Функции для бинарного кодирования
MorseEncodedResult encodeTextToMorse(const string &plaintext);
MorseDecodedResult decodeTextFromMorse(const vector<unsigned char> &binary_data);
MorseFileOperationResult encodeFileToMorse(const string &inputFilePath, const string &outputFilePath);
MorseFileOperationResult decodeFileFromMorse(const string &inputFilePath, const string &outputFilePath);

#endif // MORSE_STANDALONE_H