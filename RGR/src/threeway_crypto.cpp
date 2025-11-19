#include "../include/threeway_crypto.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <locale>
#include <sstream>
#include <iomanip>

using namespace std;

// Константы для упрощенного 3-WAY
const int THREE_WAY_BLOCK_SIZE = 12; // 96 бит = 12 байт
const int THREE_WAY_ROUNDS = 4;      // Простое количество раундов

// Вспомогательные функции

// Циклический сдвиг влево
uint32_t rotateLeft(uint32_t x, int n) {
    n = n % 32;
    return (x << n) | (x >> (32 - n));
}

// Циклический сдвиг вправо
uint32_t rotateRight(uint32_t x, int n) {
    n = n % 32;
    return (x >> n) | (x << (32 - n));
}

// ОЧЕНЬ ПРОСТАЯ обратимая функция для одного раунда
void roundFunction(uint32_t block[3], const uint32_t roundKey[3]) {
    // Простое обратимое преобразование
    uint32_t a = block[0], b = block[1], c = block[2];
    
    // Шаг 1: XOR с ключом
    a ^= roundKey[0];
    b ^= roundKey[1];
    c ^= roundKey[2];
    
    // Шаг 2: Простая перестановка
    block[0] = b;
    block[1] = c;
    block[2] = a;
    
    // Шаг 3: Простой сдвиг
    block[0] = rotateLeft(block[0], 5);
    block[1] = rotateRight(block[1], 3);
    block[2] = rotateLeft(block[2], 7);
}

// Обратная функция для одного раунда
void roundFunctionInverse(uint32_t block[3], const uint32_t roundKey[3]) {
    // Обратные операции в обратном порядке
    
    // Шаг 3: Обратные сдвиги
    block[0] = rotateRight(block[0], 5);
    block[1] = rotateLeft(block[1], 3);
    block[2] = rotateRight(block[2], 7);
    
    // Шаг 2: Обратная перестановка
    uint32_t a = block[2], b = block[0], c = block[1];
    
    // Шаг 1: XOR с ключом (обратная операция)
    block[0] = a ^ roundKey[0];
    block[1] = b ^ roundKey[1];
    block[2] = c ^ roundKey[2];
}

// Генерация раундовых ключей
void generateRoundKeys(const uint32_t key[3], uint32_t roundKeys[THREE_WAY_ROUNDS][3]) {
    uint32_t temp[3] = {key[0], key[1], key[2]};
    
    for (int round = 0; round < THREE_WAY_ROUNDS; round++) {
        // Сохраняем текущий ключ
        roundKeys[round][0] = temp[0];
        roundKeys[round][1] = temp[1];
        roundKeys[round][2] = temp[2];
        
        // Обновление ключа для следующего раунда
        temp[0] = rotateLeft(temp[0] + 0x9E3779B9, 7);
        temp[1] = rotateLeft(temp[1] + 0xB7E15162, 13);
        temp[2] = rotateLeft(temp[2] + 0xBF715880, 17);
    }
}

// Основное шифрование
void threeWayEncrypt(uint32_t block[3], const uint32_t roundKeys[THREE_WAY_ROUNDS][3]) {
    for (int round = 0; round < THREE_WAY_ROUNDS; round++) {
        roundFunction(block, roundKeys[round]);
    }
}

// Основное дешифрование
void threeWayDecrypt(uint32_t block[3], const uint32_t roundKeys[THREE_WAY_ROUNDS][3]) {
    for (int round = THREE_WAY_ROUNDS - 1; round >= 0; round--) {
        roundFunctionInverse(block, roundKeys[round]);
    }
}

// Генерация ключей
ThreeWayKeys generateThreeWayKeys() {
    ThreeWayKeys keys;
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint32_t> dist;
    
    // Генерация 96-битного ключа (3 × 32 бита)
    keys.key[0] = dist(gen);
    keys.key[1] = dist(gen);
    keys.key[2] = dist(gen);
    
    return keys;
}

// Упаковка байтов в 32-битные слова
void packBytesToBlock(const uint8_t* bytes, uint32_t block[3]) {
    block[0] = (static_cast<uint32_t>(bytes[0]) << 24) |
               (static_cast<uint32_t>(bytes[1]) << 16) |
               (static_cast<uint32_t>(bytes[2]) << 8) |
               (static_cast<uint32_t>(bytes[3]));
               
    block[1] = (static_cast<uint32_t>(bytes[4]) << 24) |
               (static_cast<uint32_t>(bytes[5]) << 16) |
               (static_cast<uint32_t>(bytes[6]) << 8) |
               (static_cast<uint32_t>(bytes[7]));
               
    block[2] = (static_cast<uint32_t>(bytes[8]) << 24) |
               (static_cast<uint32_t>(bytes[9]) << 16) |
               (static_cast<uint32_t>(bytes[10]) << 8) |
               (static_cast<uint32_t>(bytes[11]));
}

// Распаковка 32-битных слов в байты
void unpackBlockToBytes(const uint32_t block[3], uint8_t* bytes) {
    bytes[0] = static_cast<uint8_t>((block[0] >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((block[0] >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((block[0] >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(block[0] & 0xFF);
    
    bytes[4] = static_cast<uint8_t>((block[1] >> 24) & 0xFF);
    bytes[5] = static_cast<uint8_t>((block[1] >> 16) & 0xFF);
    bytes[6] = static_cast<uint8_t>((block[1] >> 8) & 0xFF);
    bytes[7] = static_cast<uint8_t>(block[1] & 0xFF);
    
    bytes[8] = static_cast<uint8_t>((block[2] >> 24) & 0xFF);
    bytes[9] = static_cast<uint8_t>((block[2] >> 16) & 0xFF);
    bytes[10] = static_cast<uint8_t>((block[2] >> 8) & 0xFF);
    bytes[11] = static_cast<uint8_t>(block[2] & 0xFF);
}

// Шифрование сообщения
vector<uint8_t> encryptMessageThreeWay(const string& message, const ThreeWayKeys& keys) {
    vector<uint8_t> encrypted;
    
    // Генерация раундовых ключей
    uint32_t roundKeys[THREE_WAY_ROUNDS][3];
    generateRoundKeys(keys.key, roundKeys);
    
    // Обработка сообщения блоками по 12 байт
    size_t messageLen = message.length();
    size_t blocks = (messageLen + THREE_WAY_BLOCK_SIZE - 1) / THREE_WAY_BLOCK_SIZE;
    
    for (size_t blockNum = 0; blockNum < blocks; blockNum++) {
        uint8_t blockBytes[THREE_WAY_BLOCK_SIZE] = {0};
        uint32_t block[3] = {0, 0, 0};
        
        // Заполнение блока данными (с паддингом если нужно)
        size_t blockStart = blockNum * THREE_WAY_BLOCK_SIZE;
        size_t bytesToCopy = min(static_cast<size_t>(THREE_WAY_BLOCK_SIZE), messageLen - blockStart);
        
        for (size_t i = 0; i < bytesToCopy; i++) {
            blockBytes[i] = static_cast<uint8_t>(message[blockStart + i]);
        }
        
        // Паддинг нулями если нужно
        for (size_t i = bytesToCopy; i < THREE_WAY_BLOCK_SIZE; i++) {
            blockBytes[i] = 0;
        }
        
        // Упаковка байтов в блок
        packBytesToBlock(blockBytes, block);
        
        // Шифрование блока
        threeWayEncrypt(block, roundKeys);
        
        // Распаковка зашифрованного блока в байты
        uint8_t encryptedBytes[THREE_WAY_BLOCK_SIZE];
        unpackBlockToBytes(block, encryptedBytes);
        
        // Добавление к результату
        for (int i = 0; i < THREE_WAY_BLOCK_SIZE; i++) {
            encrypted.push_back(encryptedBytes[i]);
        }
    }
    
    return encrypted;
}

// Дешифрование сообщения
string decryptMessageThreeWay(const vector<uint8_t>& encrypted, const ThreeWayKeys& keys) {
    string decrypted;
    
    // Генерация раундовых ключей
    uint32_t roundKeys[THREE_WAY_ROUNDS][3];
    generateRoundKeys(keys.key, roundKeys);
    
    // Обработка зашифрованных данных блоками по 12 байт
    size_t encryptedLen = encrypted.size();
    size_t blocks = encryptedLen / THREE_WAY_BLOCK_SIZE;
    
    for (size_t blockNum = 0; blockNum < blocks; blockNum++) {
        uint8_t encryptedBytes[THREE_WAY_BLOCK_SIZE];
        uint32_t block[3] = {0, 0, 0};
        
        // Заполнение блока зашифрованными данными
        size_t blockStart = blockNum * THREE_WAY_BLOCK_SIZE;
        for (size_t i = 0; i < THREE_WAY_BLOCK_SIZE; i++) {
            size_t pos = blockStart + i;
            if (pos < encryptedLen) {
                encryptedBytes[i] = encrypted[pos];
            } else {
                encryptedBytes[i] = 0;
            }
        }
        
        // Упаковка байтов в блок
        packBytesToBlock(encryptedBytes, block);
        
        // Дешифрование блока
        threeWayDecrypt(block, roundKeys);
        
        // Распаковка дешифрованного блока в байты
        uint8_t decryptedBytes[THREE_WAY_BLOCK_SIZE];
        unpackBlockToBytes(block, decryptedBytes);
        
        // Добавление всех байтов
        for (int i = 0; i < THREE_WAY_BLOCK_SIZE; i++) {
            decrypted += static_cast<char>(decryptedBytes[i]);
        }
    }
    
    // Удаляем trailing нули в конце
    while (!decrypted.empty() && decrypted.back() == 0) {
        decrypted.pop_back();
    }
    
    return decrypted;
}

// Функции для работы с файлами
void encryptFileThreeWay(const string& inputFile, const string& outputFile, const ThreeWayKeys& keys) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        throw runtime_error("Не удалось открыть входной файл: " + inputFile);
    }

    ofstream out(outputFile, ios::binary);
    if (!out) {
        throw runtime_error("Не удалось создать выходной файл: " + outputFile);
    }

    // Генерация раундовых ключей
    uint32_t roundKeys[THREE_WAY_ROUNDS][3];
    generateRoundKeys(keys.key, roundKeys);

    // Чтение и шифрование файла блоками
    vector<uint8_t> buffer(THREE_WAY_BLOCK_SIZE);
    while (in.read(reinterpret_cast<char*>(buffer.data()), THREE_WAY_BLOCK_SIZE)) {
        size_t bytesRead = static_cast<size_t>(in.gcount());
        
        // Паддинг последнего блока если нужно
        if (bytesRead < THREE_WAY_BLOCK_SIZE) {
            fill(buffer.begin() + bytesRead, buffer.end(), 0);
        }
        
        uint32_t block[3] = {0, 0, 0};
        
        // Упаковка байтов в блок
        packBytesToBlock(buffer.data(), block);
        
        // Шифрование блока
        threeWayEncrypt(block, roundKeys);
        
        // Распаковка и запись зашифрованного блока
        uint8_t encryptedBytes[THREE_WAY_BLOCK_SIZE];
        unpackBlockToBytes(block, encryptedBytes);
        
        out.write(reinterpret_cast<char*>(encryptedBytes), THREE_WAY_BLOCK_SIZE);
    }

    in.close();
    out.close();
}

void decryptFileThreeWay(const string& inputFile, const string& outputFile, const ThreeWayKeys& keys) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        throw runtime_error("Не удалось открыть входной файл: " + inputFile);
    }

    ofstream out(outputFile, ios::binary);
    if (!out) {
        throw runtime_error("Не удалось создать выходной файл: " + outputFile);
    }

    // Генерация раундовых ключей
    uint32_t roundKeys[THREE_WAY_ROUNDS][3];
    generateRoundKeys(keys.key, roundKeys);

    // Чтение и дешифрование файла блоками
    vector<uint8_t> buffer(THREE_WAY_BLOCK_SIZE);
    while (in.read(reinterpret_cast<char*>(buffer.data()), THREE_WAY_BLOCK_SIZE)) {
        uint32_t block[3] = {0, 0, 0};
        
        // Упаковка байтов в блок
        packBytesToBlock(buffer.data(), block);
        
        // Дешифрование блока
        threeWayDecrypt(block, roundKeys);
        
        // Распаковка и запись дешифрованного блока
        uint8_t decryptedBytes[THREE_WAY_BLOCK_SIZE];
        unpackBlockToBytes(block, decryptedBytes);
        
        out.write(reinterpret_cast<char*>(decryptedBytes), THREE_WAY_BLOCK_SIZE);
    }

    in.close();
    out.close();
}

// Функция для проверки корректности ключей
bool validateThreeWayKeys(const ThreeWayKeys& keys) {
    string test_msg = "Test 3-WAY!";
    try {
        vector<uint8_t> encrypted = encryptMessageThreeWay(test_msg, keys);
        string decrypted = decryptMessageThreeWay(encrypted, keys);
        return (test_msg == decrypted);
    } catch (...) {
        return false;
    }
}

// Функция для генерации и сохранения ключей
void generateAndSaveKeys() {
    ThreeWayKeys keys = generateThreeWayKeys();
    
    cout << "Сгенерированы новые ключи 3-WAY:\n";
    cout << "Ключ (96 бит): ";
    for (int i = 0; i < 3; i++) {
        cout << hex << setw(8) << setfill('0') << keys.key[i] << " ";
    }
    cout << dec << "\n\n";
    
    // Проверка корректности ключей с детальной отладкой
    cout << "Проверка ключей:\n";
    string test_msg = "Hello 3-WAY!";
    cout << "Исходное сообщение: '" << test_msg << "'" << endl;
    
    vector<uint8_t> encrypted = encryptMessageThreeWay(test_msg, keys);
    cout << "Зашифрованное сообщение (hex): ";
    for (size_t i = 0; i < encrypted.size(); i++) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(encrypted[i]) << " ";
    }
    cout << dec << endl;
    
    string decrypted = decryptMessageThreeWay(encrypted, keys);
    cout << "Расшифрованное сообщение: '" << decrypted << "'" << endl;
    
    if (test_msg == decrypted) {
        cout << "✓ ТЕСТ ПРОЙДЕН УСПЕШНО\n";
    } else {
        cout << "✗ ТЕСТ НЕ ПРОЙДЕН\n";
        cout << "Длина исходного: " << test_msg.length() << ", длина расшифрованного: " << decrypted.length() << endl;
        
        // Детальная отладка
        cout << "Детальная отладка:\n";
        cout << "Исходные байты: ";
        for (char c : test_msg) {
            cout << hex << setw(2) << setfill('0') << (int)(unsigned char)c << " ";
        }
        cout << dec << endl;
        
        cout << "Расшифрованные байты: ";
        for (char c : decrypted) {
            cout << hex << setw(2) << setfill('0') << (int)(unsigned char)c << " ";
        }
        cout << dec << endl;
    }
    cout << endl;
    
    ofstream keyFile("threeway_keys.txt");
    if (keyFile) {
        keyFile << "3-WAY Keys:\n";
        for (int i = 0; i < 3; i++) {
            keyFile << "Key Part " << i << ": " << hex << setw(8) << setfill('0') << keys.key[i] << dec << "\n";
        }
        keyFile.close();
        cout << "Ключи сохранены в файл: threeway_keys.txt\n";
    }
}

// Остальные функции остаются без изменений
bool getKeyManual(ThreeWayKeys& keys) {
    cout << "Введите ключ 3-WAY (3 шестнадцатеричных числа через пробел):\n";
    string keyStr;
    getline(cin, keyStr);
    
    istringstream iss(keyStr);
    for (int i = 0; i < 3; i++) {
        string hexStr;
        if (!(iss >> hexStr)) {
            cout << "Ошибка: недостаточно чисел в ключе\n";
            return false;
        }
        
        try {
            keys.key[i] = stoul(hexStr, nullptr, 16);
        } catch (const exception& e) {
            cout << "Ошибка в формате ключа: " << hexStr << "\n";
            return false;
        }
    }
    
    return true;
}

void printEncryptedMessage(const vector<uint8_t>& encrypted) {
    cout << "Зашифрованное сообщение (hex): ";
    for (size_t i = 0; i < encrypted.size(); i++) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(encrypted[i]) << " ";
    }
    cout << dec << endl;
    
    cout << "Зашифрованное сообщение (dec): ";
    for (size_t i = 0; i < encrypted.size(); i++) {
        cout << setw(3) << static_cast<int>(encrypted[i]) << " ";
    }
    cout << endl;
}

vector<uint8_t> readEncryptedMessage(const string& encryptedStr) {
    vector<uint8_t> encrypted;
    istringstream iss(encryptedStr);
    
    string token;
    while (iss >> token) {
        try {
            int value = stoi(token);
            if (value < 0 || value > 255) {
                throw out_of_range("Value out of range");
            }
            encrypted.push_back(static_cast<uint8_t>(value));
        } catch (...) {
            try {
                int value = stoi(token, nullptr, 16);
                if (value < 0 || value > 255) {
                    throw out_of_range("Value out of range");
                }
                encrypted.push_back(static_cast<uint8_t>(value));
            } catch (...) {
                throw runtime_error("Неверный формат числа: " + token);
            }
        }
    }
    
    return encrypted;
}

// ГЛАВНАЯ ФУНКЦИЯ
extern "C" {

void run_threeway_crypto() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << "=== 3-WAY Шифрование/Дешифрование ===" << endl;
    cout << "0. Выход в главное меню " << endl;
    cout << "1. Сгенерировать и сохранить ключи" << endl;
    cout << "2. Шифровать сообщение" << endl;
    cout << "3. Дешифровать сообщение" << endl;
    cout << "4. Шифровать файл" << endl;
    cout << "5. Дешифровать файл" << endl;
    cout << "Выберите действие: ";

    int choice;
    cin >> choice;
    cin.ignore();

    try {
        switch (choice) {
            case 0:
                cout << "Выход из режима 3-WAY." << endl;
                break;
            case 1: {
                generateAndSaveKeys();
                break;
            }
            case 2: {
                cout << "Использовать автоматические (a) или ручной ввод ключа (m)? [a/m]: ";
                char keyChoice;
                cin >> keyChoice;
                cin.ignore();

                ThreeWayKeys keys;
                
                if (keyChoice == 'm' || keyChoice == 'M') {
                    cout << "Введите ключ для шифрования:\n";
                    if (!getKeyManual(keys)) {
                        cout << "Ошибка ввода ключа!\n";
                        break;
                    }
                } else {
                    keys = generateThreeWayKeys();
                    cout << "Используются автоматически сгенерированные ключи:\n";
                    cout << "Ключ: ";
                    for (int i = 0; i < 3; i++) {
                        cout << hex << setw(8) << setfill('0') << keys.key[i] << " ";
                    }
                    cout << dec << endl;
                }

                cout << "Введите сообщение для шифрования: ";
                string message;
                getline(cin, message);

                vector<uint8_t> encrypted = encryptMessageThreeWay(message, keys);
                printEncryptedMessage(encrypted);
                
                // Автоматическая проверка дешифрования
                string test_decrypted = decryptMessageThreeWay(encrypted, keys);
                cout << "Тест дешифрования: '" << test_decrypted << "'" << endl;
                
                break;
            }
            case 3: {
                ThreeWayKeys keys;
                cout << "Введите ключ для дешифрования:\n";
                if (!getKeyManual(keys)) {
                    cout << "Ошибка ввода ключа!\n";
                    break;
                }

                cout << "Введите зашифрованное сообщение (числа через пробел, dec или hex): ";
                string encryptedStr;
                getline(cin, encryptedStr);

                vector<uint8_t> encrypted = readEncryptedMessage(encryptedStr);
                string decrypted = decryptMessageThreeWay(encrypted, keys);
                cout << "Расшифрованное сообщение: '" << decrypted << "'" << endl;
                break;
            }
            case 4: {
                cout << "Использовать автоматические (a) или ручной ввод ключа (m)? [a/m]: ";
                char keyChoice;
                cin >> keyChoice;
                cin.ignore();

                ThreeWayKeys keys;
                
                if (keyChoice == 'm' || keyChoice == 'M') {
                    cout << "Введите ключ для шифрования:\n";
                    if (!getKeyManual(keys)) {
                        cout << "Ошибка ввода ключа!\n";
                        break;
                    }
                } else {
                    keys = generateThreeWayKeys();
                }

                cout << "Введите имя файла для шифрования: ";
                string inputFile;
                getline(cin, inputFile);

                cout << "Введите имя выходного файла: ";
                string outputFile;
                getline(cin, outputFile);

                encryptFileThreeWay(inputFile, outputFile, keys);
                cout << "Файл успешно зашифрован." << endl;
                break;
            }
            case 5: {
                ThreeWayKeys keys;
                cout << "Введите ключ для дешифрования:\n";
                if (!getKeyManual(keys)) {
                    cout << "Ошибка ввода ключа!\n";
                    break;
                }

                cout << "Введите имя файла для дешифрования: ";
                string inputFile;
                getline(cin, inputFile);

                cout << "Введите имя выходного файла: ";
                string outputFile;
                getline(cin, outputFile);

                decryptFileThreeWay(inputFile, outputFile, keys);
                cout << "Файл успешно расшифрован." << endl;
                break;
            }
            default:
                cout << "Неверный выбор." << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
    
    cout << "Нажмите Enter для возврата в главное меню...";
    cin.get();
}

} // extern "C"