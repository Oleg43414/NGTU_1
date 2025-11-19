#include "../include/morse_standalone.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <map>
#include <fstream>
#include <cstdint>
#include <cstring>

using namespace std;

// ==================== КЛАСС MorseCode ====================

MorseCode::MorseCode() {
    initializeMaps();
}

void MorseCode::initializeMaps() {
    morseMap.clear();
    reverseMap.clear();
    
    // ENGLISH ALPHABET
    morseMap['A'] = ".-";
    morseMap['B'] = "-...";
    morseMap['C'] = "-.-.";
    morseMap['D'] = "-..";
    morseMap['E'] = ".";
    morseMap['F'] = "..-.";
    morseMap['G'] = "--.";
    morseMap['H'] = "....";
    morseMap['I'] = "..";
    morseMap['J'] = ".---";
    morseMap['K'] = "-.-";
    morseMap['L'] = ".-..";
    morseMap['M'] = "--";
    morseMap['N'] = "-.";
    morseMap['O'] = "---";
    morseMap['P'] = ".--.";
    morseMap['Q'] = "--.-";
    morseMap['R'] = ".-.";
    morseMap['S'] = "...";
    morseMap['T'] = "-";
    morseMap['U'] = "..-";
    morseMap['V'] = "...-";
    morseMap['W'] = ".--";
    morseMap['X'] = "-..-";
    morseMap['Y'] = "-.--";
    morseMap['Z'] = "--..";
    
    //  ЧИСЛА
    morseMap['0'] = "-----";
    morseMap['1'] = ".----";
    morseMap['2'] = "..---";
    morseMap['3'] = "...--";
    morseMap['4'] = "....-";
    morseMap['5'] = ".....";
    morseMap['6'] = "-....";
    morseMap['7'] = "--...";
    morseMap['8'] = "---..";
    morseMap['9'] = "----.";
    
    // СПЕЦ СИМВОЛЫ
    morseMap['.'] = ".-.-.-";
    morseMap[','] = "--..--";
    morseMap['?'] = "..--..";
    morseMap['!'] = "-.-.--";
    morseMap['@'] = ".--.-.";
    morseMap['/'] = "-..-.";
    
    // СОЗДАТЬ ОБРАТНУЮ КАРТУ
    for (const auto& pair : morseMap) {
        reverseMap[pair.second] = pair.first;
    }
    
    // Добавить место отдельно
    reverseMap["/"] = ' ';
}

string MorseCode::encodeChar(char c) const {
    if (c == ' ') {
        return "/";
    }
    
    char upperChar = toupper(c);
    auto it = morseMap.find(upperChar);
    if (it != morseMap.end()) {
        return it->second;
    }
    
    return "";
}

string MorseCode::encodeString(const string& text) const {
    string result;
    bool firstChar = true;
    
    for (char c : text) {
        string morseChar = encodeChar(c);
        
        if (!morseChar.empty()) {
            if (!firstChar) {
                result += " ";
            }
            result += morseChar;
            firstChar = false;
        }
    }
    
    return result;
}

char MorseCode::decodeChar(const string& morse) const {
    // Сначала проверьте наличие места
    if (morse == "/") {
        return ' ';
    }
    
    auto it = reverseMap.find(morse);
    if (it != reverseMap.end()) {
        return it->second;
    }
    
    return '?';
}

string MorseCode::decodeString(const string& morse) const {
    string result;
    string token;
    
    // Разобрать строку кода Морзе
    for (size_t i = 0; i < morse.length(); i++) {
        char c = morse[i];
        
        if (c == ' ') {
            // Конец символа Морзе
            if (!token.empty()) {
                char decoded = decodeChar(token);
                result += decoded;
                token.clear();
            }
        } else {
            token += c;
        }
    }
    
    // Обработать последний токен
    if (!token.empty()) {
        char decoded = decodeChar(token);
        result += decoded;
    }
    
    return result;
}

bool MorseCode::isSupported(char c) const {
    if (c == ' ') return true;
    char upperChar = toupper(c);
    return morseMap.find(upperChar) != morseMap.end();
}

void MorseCode::printSupportedChars() const {
    cout << "=== ПОДДЕРЖИВАЕМЫЕ СИМВОЛЫ ===" << endl;
    cout << "Буквы: ABCDEFGHIJKLMNOPQRSTUVWXYZ" << endl;
    cout << "Числа: 0123456789" << endl;
    cout << "спец символы: . , ? ! @ /" << endl;
    cout << "Пробел: /" << endl;
}

void MorseCode::runDemo() const {
    cout << "=== КОД МОРЗЕ ДЕМОНСТРАЦИОННЫЙ ===" << endl;
    
    // Проверка конкретных случаев
    vector<pair<string, string>> tests = {
        {"SOS", "... --- ..."},
        {"HELLO", ".... . .-.. .-.. ---"},
        {"WORLD", ".-- --- .-. .-.. -.."},
        {"123", ".---- ..--- ...--"},
        {"I LOVE YOU", ".. / .-.. --- ...- . / -.-- --- ..-"},
        {"HELLO WORLD", ".... . .-.. .-.. --- / .-- --- .-. .-.. -.."}
    };
    
    for (const auto& test : tests) {
        cout << "\nTest: " << test.first << endl;
        cout << "Ожидаемый Морзе: " << test.second << endl;
        
        // Кодировать
        string encoded = encodeString(test.first);
        cout << "Кодировать: " << encoded << endl;
        
        // Декрдировать 
        string decoded = decodeString(test.second);
        cout << "Декодировать: " << decoded << endl;
        
        bool encodeOk = (encoded == test.second);
        bool decodeOk = (decoded == test.first);
        
        cout << "Кодировать: " << (encodeOk ? "OK" : "FAIL") << endl;
        cout << "Декодировать: " << (decodeOk ? "OK" : "FAIL") << endl;
    }
}

// ==================== БИНАРНОЕ КОДИРОВАНИЕ ====================

static const map<unsigned char, string> nibble_to_morse = {
    {0x0, "."},    {0x1, "-"},    {0x2, ".."},   {0x3, ".-"},
    {0x4, "-."},   {0x5, "--"},   {0x6, "..."},  {0x7, "..-"},
    {0x8, ".-."},  {0x9, ".--"},  {0xA, "-.."},  {0xB, "-.-"},
    {0xC, "--."},  {0xD, "---"},  {0xE, "...."}, {0xF, "...-"}
};

static map<string, unsigned char> morse_to_nibble;
static bool nibble_map_initialized = false;

static void init_nibble_map() {
    if (!nibble_map_initialized) {
        for (const auto& pair : nibble_to_morse) {
            morse_to_nibble[pair.second] = pair.first;
        }
        nibble_map_initialized = true;
    }
}

const string DOT_BITS = "1";
const string DASH_BITS = "111";
const string ELEMENT_GAP = "0";
const string PART_GAP = "000";
const string BYTE_GAP = "0000000";

string morse_to_bits(const string& morse) {
    string bits;
    for (size_t i = 0; i < morse.length(); ++i) {
        bits += (morse[i] == '.') ? DOT_BITS : DASH_BITS;
        if (i < morse.length() - 1) {
            bits += ELEMENT_GAP;
        }
    }
    return bits;
}

MorseEncodedResult encodeTextToMorse(const string &text) {
    MorseEncodedResult result;
    string bit_string;

    for (size_t i = 0; i < text.length(); ++i) {
        unsigned char byte = text[i];
        unsigned char high = (byte >> 4) & 0x0F;
        unsigned char low = byte & 0x0F;

        bit_string += morse_to_bits(nibble_to_morse.at(high));
        bit_string += PART_GAP;
        bit_string += morse_to_bits(nibble_to_morse.at(low));

        if (i < text.length() - 1) {
            bit_string += BYTE_GAP;
        }
    }

    vector<unsigned char> packed;
    uint64_t total_bits = bit_string.length();
    
    packed.resize(sizeof(total_bits));
    memcpy(packed.data(), &total_bits, sizeof(total_bits));

    unsigned char current = 0;
    int bit_count = 0;
    for (char bit : bit_string) {
        current = (current << 1) | (bit - '0');
        bit_count++;
        if (bit_count == 8) {
            packed.push_back(current);
            bit_count = 0;
        }
    }
    if (bit_count > 0) {
        current <<= (8 - bit_count);
        packed.push_back(current);
    }

    result.binary_data = packed;
    result.success = true;
    return result;
}

MorseDecodedResult decodeTextFromMorse(const vector<unsigned char> &data) {
    MorseDecodedResult result;
    init_nibble_map();

    if (data.size() < sizeof(uint64_t)) {
        result.success = false;
        result.error_message = "Данные слишком короткие";
        return result;
    }

    uint64_t total_bits;
    memcpy(&total_bits, data.data(), sizeof(total_bits));

    string bit_string;
    for (size_t i = sizeof(total_bits); i < data.size(); ++i) {
        for (int j = 7; j >= 0; --j) {
            bit_string += ((data[i] >> j) & 1) ? '1' : '0';
        }
    }
    if (bit_string.length() > total_bits) {
        bit_string.resize(total_bits);
    }
    
    string text_result;
    string current_morse;
    unsigned char current_byte = 0;
    bool high_nibble = true;

    for (size_t i = 0; i < bit_string.length(); ) {
        size_t gap = bit_string.find('0', i);
        if (gap == string::npos) {
            gap = bit_string.length();
        }

        string element = bit_string.substr(i, gap - i);
        if (element == DOT_BITS) current_morse += '.';
        else if (element == DASH_BITS) current_morse += '-';

        i = gap;
        
        size_t zeros = 0;
        while(i < bit_string.length() && bit_string[i] == '0') {
            zeros++;
            i++;
        }

        if (zeros >= 3 || i >= bit_string.length()) {
            if (morse_to_nibble.count(current_morse)) {
                unsigned char nibble = morse_to_nibble.at(current_morse);
                if (high_nibble) {
                    current_byte = (nibble << 4);
                    high_nibble = false;
                } else {
                    current_byte |= nibble;
                    text_result += static_cast<char>(current_byte);
                    high_nibble = true;
                }
            }
            current_morse.clear();
        }
    }

    result.plaintext = text_result;
    result.success = true;
    return result;
}

MorseFileOperationResult encodeFileToMorse(const string &input_path, const string &output_path) {
    ifstream input(input_path, ios::binary);
    if (!input) {
        return {false, "Невозможно открыть выходной файл"};
    }
    
    string content((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    MorseEncodedResult encoded = encodeTextToMorse(content);

    ofstream output(output_path, ios::binary);
    if (!output) {
        return {false, "Невозможно открыть выходной файл"};
    }

    output.write(reinterpret_cast<const char*>(encoded.binary_data.data()), encoded.binary_data.size());
    output.close();
    
    return {true, "Файл успешно закодирован"};
}

MorseFileOperationResult decodeFileFromMorse(const string &input_path, const string &output_path) {
    ifstream input(input_path, ios::binary);
    if (!input) {
        return {false, "Невозможно открыть входной файл"};
    }

    vector<unsigned char> content((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    MorseDecodedResult decoded = decodeTextFromMorse(content);
    if (!decoded.success) {
        return {false, decoded.error_message};
    }

    ofstream output(output_path, ios::binary);
    if (!output) {
        return {false, "Не могу открыть выходной файл"};
    }

    output << decoded.plaintext;
    output.close();

    return {true, "Файл успешно декодирован"};
}

// ==================== C-ИНТЕРФЕЙС ДЛЯ БИБЛИОТЕКИ ====================

extern "C" {

void run_morse_demo() {
    MorseCode morse;
    morse.runDemo();
    
    // Дополнительная интерактивная часть
    cout << "\n=== ИНТЕРАКТИВНЫЙ РЕЖИМ МОРЗЕ ===" << endl;
    
    int choice;
    do {
        cout << "0. Выход в главное меню" << endl;
        cout << "1. Закодировать текст в Морзе" << endl;
        cout << "2. Декодировать Морзе в текст" << endl;
        cout << "3. Показать поддерживаемые символы" << endl;
        cout << "4. Бинарное кодирование файла" << endl;
        cout << "5. Бинарное декодирование файла" << endl;
        cout << "Выберите действие: ";
        cin >> choice;
        cin.ignore();
        
        MorseCode morseInstance;
        
        switch (choice) {
            case 1: {
                string text;
                cout << "Введите текст для кодирования: ";
                getline(cin, text);
                
                string encoded = morseInstance.encodeString(text);
                cout << "Закодированный текст: " << encoded << endl;
                break;
            }
                
            case 2: {
                string morseCode;
                cout << "Введите код Морзе для декодирования: ";
                getline(cin, morseCode);
                
                string decoded = morseInstance.decodeString(morseCode);
                cout << "Декодированный текст: " << decoded << endl;
                break;
            }
                
            case 3:
                morseInstance.printSupportedChars();
                break;
                
            case 4: {
                string inputFile, outputFile;
                cout << "Введите имя входного файла: ";
                getline(cin, inputFile);
                cout << "Введите имя выходного файла: ";
                getline(cin, outputFile);
                
                MorseFileOperationResult result = encodeFileToMorse(inputFile, outputFile);
                cout << (result.success ? "✓ Успех: " : "✗ Ошибка: ") << result.message << endl;
                break;
            }
                
            case 5: {
                string inputFile, outputFile;
                cout << "Введите имя входного файла: ";
                getline(cin, inputFile);
                cout << "Введите имя выходного файла: ";
                getline(cin, outputFile);
                
                MorseFileOperationResult result = decodeFileFromMorse(inputFile, outputFile);
                cout << (result.success ? "✓ Успех: " : "✗ Ошибка: ") << result.message << endl;
                break;
            }
                
            case 0:
                cout << "Выход из режима Морзе." << endl;
                break;
                
            default:
                cout << "Неверный выбор!" << endl;
        }
    } while (choice != 0);
}

} // extern "C"