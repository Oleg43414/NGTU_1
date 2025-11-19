#include "../include/rsa_crypto.h"
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

using namespace std;

// Вспомогательные функции (без RSA_API)

int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int64_t lcm(int64_t a, int64_t b) {
    return (a / gcd(a, b)) * b; 
}

int64_t modInverse(int64_t a, int64_t m) {
    int64_t m0 = m;
    int64_t y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        int64_t q = a / m;
        int64_t t = m;

        m = a % m;
        a = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0)
        x += m0;

    return x;
}

int64_t powmod(int64_t a, int64_t b, int64_t mod) {
    int64_t res = 1;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1)
            res = (res * a) % mod;
        a = (a * a) % mod;
        b = b >> 1;
    }
    return res;
}

// Основные функции RSA с RSA_API

RSA_API bool isPrime(int64_t n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    int64_t d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int64_t> dist(2, n - 2);

    for (int i = 0; i < k; i++) {
        int64_t a = dist(gen);
        int64_t x = powmod(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;

            if (x == 1) return false;
            if (x == n - 1) break;
        }

        if (x != n - 1) return false;
    }

    return true;
}

RSA_API int64_t generatePrime(int64_t min, int64_t max) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int64_t> dist(min, max);

    int64_t num;
    do {
        num = dist(gen);
        if (num % 2 == 0 && num != 2)
            num++;
    } while (!isPrime(num, 5));

    return num;
}

RSA_API RSAKeys generateRSAKeys() {
    RSAKeys keys;

    // Генерация простых чисел p и q
    int64_t p = generatePrime(1000, 10000);
    int64_t q = generatePrime(1000, 10000);

    while (p == q) {
        q = generatePrime(1000, 10000);
    }

    // Вычисление модуля n = p * q
    keys.n = p * q;

    // Вычисление функции Эйлера φ(n) = (p-1)*(q-1)
    int64_t phi = (p - 1) * (q - 1);

    // Выбор открытой экспоненты e (должна быть взаимно простой с φ(n))
    int64_t e = 65537;
    while (gcd(e, phi) != 1) {
        e++;
        if (e > phi) e = 3; // Защита от зацикливания
    }
    keys.publicKey = e;

    // Вычисление секретной экспоненты d = e^(-1) mod φ(n)
    keys.privateKey = modInverse(e, phi);

    return keys;
}

// НАДЕЖНАЯ реализация шифрования с обработкой UTF-8
RSA_API vector<int64_t> encryptMessageRSA(const string& message, int64_t e, int64_t n) {
    vector<int64_t> encrypted;

    for (unsigned char c : message) {
        int64_t m = static_cast<int64_t>(c);
        int64_t encrypted_char = powmod(m, e, n);
        
        // Гарантируем корректный диапазон
        encrypted_char = encrypted_char % n;
        if (encrypted_char < 0) encrypted_char += n;
        
        encrypted.push_back(encrypted_char);
    }

    return encrypted;
}

// НАДЕЖНАЯ реализация дешифрования с обработкой UTF-8
RSA_API string decryptMessageRSA(const vector<int64_t>& encrypted, int64_t d, int64_t n) {
    string decrypted;

    for (int64_t num : encrypted) {
        int64_t m = powmod(num, d, n);
        
        // КРИТИЧЕСКО ВАЖНО: правильное приведение к байту
        m = m % 256;
        if (m < 0) m += 256;
        
        // Дополнительная проверка
        if (m < 0 || m > 255) {
            m = 0; // Защита от некорректных значений
        }
        
        decrypted += static_cast<unsigned char>(m);
    }

    return decrypted;
}

RSA_API void encryptFileRSA(const string& inputFile, const string& outputFile, int64_t e, int64_t n) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        throw runtime_error("Не удалось открыть входной файл: " + inputFile);
    }

    ofstream out(outputFile, ios::binary);
    if (!out) {
        throw runtime_error("Не удалось создать выходной файл: " + outputFile);
    }

    unsigned char c;
    while (in.read(reinterpret_cast<char*>(&c), 1)) {
        int64_t m = static_cast<int64_t>(c);
        int64_t encrypted = powmod(m, e, n);
        encrypted = encrypted % n;
        if (encrypted < 0) encrypted += n;
        out << encrypted << " ";
    }

    in.close();
    out.close();
}

RSA_API void decryptFileRSA(const string& inputFile, const string& outputFile, int64_t d, int64_t n) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        throw runtime_error("Не удалось открыть входной файл: " + inputFile);
    }

    ofstream out(outputFile, ios::binary);
    if (!out) {
        throw runtime_error("Не удалось создать выходной файл: " + outputFile);
    }

    int64_t num;
    while (in >> num) {
        int64_t decrypted = powmod(num, d, n);
        decrypted = decrypted % 256;
        if (decrypted < 0) decrypted += 256;
        if (decrypted < 0 || decrypted > 255) decrypted = 0;
        
        unsigned char c = static_cast<unsigned char>(decrypted);
        out.write(reinterpret_cast<const char*>(&c), 1);
    }

    in.close();
    out.close();
}

// Функция для проверки корректности ключей
bool validateKeys(int64_t e, int64_t d, int64_t n) {
    // Простая проверка: шифруем и дешифруем тестовое сообщение
    int64_t test_msg = 65; // 'A'
    try {
        int64_t encrypted = powmod(test_msg, e, n);
        int64_t decrypted = powmod(encrypted, d, n);
        return (test_msg == decrypted % 256);
    } catch (...) {
        return false;
    }
}

// Функция для генерации и сохранения ключей
RSA_API void generateAndSaveKeys() {
    RSAKeys keys = generateRSAKeys();
    
    cout << "Сгенерированы новые ключи RSA:\n";
    cout << "Открытый ключ (e, n): (" << keys.publicKey << ", " << keys.n << ")\n";
    cout << "Закрытый ключ (d, n): (" << keys.privateKey << ", " << keys.n << ")\n\n";
    
    // Проверка корректности ключей
    cout << "Проверка ключей:\n";
    int64_t test_msg = 65; // 'A'
    int64_t encrypted = powmod(test_msg, keys.publicKey, keys.n);
    int64_t decrypted = powmod(encrypted, keys.privateKey, keys.n);
    cout << "Тест: " << test_msg << " -> " << encrypted << " -> " << decrypted;
    if (test_msg == decrypted % 256) {
        cout << " ✓ OK\n";
    } else {
        cout << " ✗ ERROR\n";
    }
    cout << endl;
    
    ofstream keyFile("rsa_keys.txt");
    if (keyFile) {
        keyFile << "RSA Keys:\n";
        keyFile << "Public Key (e): " << keys.publicKey << "\n";
        keyFile << "Private Key (d): " << keys.privateKey << "\n";
        keyFile << "Modulus (n): " << keys.n << "\n";
        keyFile.close();
        cout << "Ключи сохранены в файл: rsa_keys.txt\n";
    }
}

// Функция для ручного ввода ОТКРЫТОГО ключа (для шифрования)
RSA_API bool getPublicKeyManual(int64_t& e, int64_t& n) {
    cout << "Введите открытый ключ e: ";
    if (!(cin >> e)) return false;
    
    cout << "Введите модуль n: ";
    if (!(cin >> n)) return false;
    
    // Проверка корректности введенных значений
    if (e <= 1 || n <= 1) {
        cout << "Ошибка: ключи должны быть положительными числами больше 1\n";
        return false;
    }
    
    cin.ignore();
    return true;
}

// Функция для ручного ввода ЗАКРЫТОГО ключа (для дешифрования)
RSA_API bool getPrivateKeyManual(int64_t& d, int64_t& n) {
    cout << "Введите закрытый ключ d: ";
    if (!(cin >> d)) return false;
    
    cout << "Введите модуль n: ";
    if (!(cin >> n)) return false;
    
    // Проверка корректности введенных значений
    if (d <= 1 || n <= 1) {
        cout << "Ошибка: ключи должны быть положительными числами больше 1\n";
        return false;
    }
    
    cin.ignore();
    return true;
}

// Функция для ручного ввода ПАРЫ ключей (для полной проверки)
RSA_API bool getKeyPairManual(int64_t& e, int64_t& d, int64_t& n) {
    cout << "Введите открытый ключ e: ";
    if (!(cin >> e)) return false;
    
    cout << "Введите закрытый ключ d: ";
    if (!(cin >> d)) return false;
    
    cout << "Введите модуль n: ";
    if (!(cin >> n)) return false;
    
    // Проверка корректности ключей
    if (e <= 1 || d <= 1 || n <= 1) {
        cout << "Ошибка: ключи должны быть положительными числами больше 1\n";
        return false;
    }
    
    // Проверка совместимости ключей
    cout << "Проверка ключей... ";
    if (validateKeys(e, d, n)) {
        cout << "✓ Ключи корректны\n";
    } else {
        cout << "✗ Ошибка: ключи несовместимы!\n";
        cout << "Рекомендуется использовать автоматическую генерацию ключей\n";
        return false;
    }
    
    cin.ignore();
    return true;
}

RSA_API void run_rsa_crypto() {
    cout << "RSA Шифрование/Дешифрование\n";
    cout << "0. Выход в главное меню\n";
    cout << "1. Сгенерировать и сохранить ключи\n";
    cout << "2. Шифровать сообщение\n";
    cout << "3. Дешифровать сообщение\n";
    cout << "4. Шифровать файл\n";
    cout << "5. Дешифровать файл\n";
    cout << "Выберите действие: ";

    int choice;
    cin >> choice;
    cin.ignore();

    try {
        switch (choice) {
            case 0:
                cout << "Выход из режима RSA." << endl;
                break;
            case 1: {
                generateAndSaveKeys();
                break;
            }
            case 2: {
                cout << "Использовать автоматические (a) или ручной ввод ключей (m)? [a/m]: ";
                char keyChoice;
                cin >> keyChoice;
                cin.ignore();

                int64_t e, n;
                RSAKeys autoKeys;
                
                if (keyChoice == 'm' || keyChoice == 'M') {
                    cout << "Введите ОТКРЫТЫЙ ключ для шифрования:\n";
                    if (!getPublicKeyManual(e, n)) {
                        cout << "Ошибка ввода ключей!\n";
                        break;
                    }
                } else {
                    autoKeys = generateRSAKeys();
                    e = autoKeys.publicKey;
                    n = autoKeys.n;
                    cout << "Используются автоматически сгенерированные ключи:\n";
                    cout << "Открытый ключ (e, n): (" << e << ", " << n << ")\n";
                }

                cout << "Введите сообщение для шифрования: ";
                string message;
                getline(cin, message);

                vector<int64_t> encrypted = encryptMessageRSA(message, e, n);
                cout << "Зашифрованное сообщение: ";
                for (int64_t num : encrypted) {
                    cout << num << " ";
                }
                cout << endl;
                
                // ТЕСТ дешифрования если ключи автоматические
                if (keyChoice != 'm' && keyChoice != 'M') {
                    string test_decrypted = decryptMessageRSA(encrypted, autoKeys.privateKey, autoKeys.n);
                    cout << "Тест дешифрования: " << test_decrypted << endl;
                } else {
                    cout << "Для дешифрования используйте закрытый ключ, соответствующий введенному открытому ключу\n";
                }
                break;
            }
            case 3: {
                cout << "Рекомендуется ввести полную пару ключей для проверки\n";
                cout << "Ввести только закрытый ключ (a) или полную пару с проверкой (m)? [a/m]: ";
                char keyType;
                cin >> keyType;
                cin.ignore();

                int64_t d, n, e;
                bool keysValid = false;

                if (keyType == '2') {
                    cout << "Введите полную пару ключей:\n";
                    if (getKeyPairManual(e, d, n)) {
                        keysValid = true;
                    }
                } else {
                    cout << "Введите ЗАКРЫТЫЙ ключ для дешифрования:\n";
                    if (getPrivateKeyManual(d, n)) {
                        keysValid = true;
                    }
                }

                if (!keysValid) {
                    cout << "Ошибка ввода ключей!\n";
                    break;
                }

                cout << "Введите зашифрованное сообщение (числа через пробел): ";
                string encryptedStr;
                getline(cin, encryptedStr);

                vector<int64_t> encrypted;
                size_t pos = 0;
                while (pos < encryptedStr.length()) {
                    size_t spacePos = encryptedStr.find(' ', pos);
                    if (spacePos == string::npos) spacePos = encryptedStr.length();

                    string numStr = encryptedStr.substr(pos, spacePos - pos);
                    if (!numStr.empty()) {
                        try {
                            encrypted.push_back(stoll(numStr));
                        } catch (const exception& e) {
                            throw runtime_error("Неверный формат числа: " + numStr);
                        }
                    }
                    pos = spacePos + 1;
                }

                string decrypted = decryptMessageRSA(encrypted, d, n);
                cout << "Расшифрованное сообщение: " << decrypted << endl;
                break;
            }
            case 4: {
                cout << "Использовать автоматические (a) или ручной ввод ключей (m)? [a/m]: ";
                char keyChoice;
                cin >> keyChoice;
                cin.ignore();

                int64_t e, n;
                
                if (keyChoice == 'm' || keyChoice == 'M') {
                    cout << "Введите ОТКРЫТЫЙ ключ для шифрования:\n";
                    if (!getPublicKeyManual(e, n)) {
                        cout << "Ошибка ввода ключей!\n";
                        break;
                    }
                } else {
                    RSAKeys keys = generateRSAKeys();
                    e = keys.publicKey;
                    n = keys.n;
                    cout << "Используются автоматически сгенерированные ключи:\n";
                    cout << "Открытый ключ (e, n): (" << e << ", " << n << ")\n";
                }

                cout << "Введите имя файла для шифрования: ";
                string inputFile;
                getline(cin, inputFile);

                cout << "Введите имя выходного файла: ";
                string outputFile;
                getline(cin, outputFile);

                encryptFileRSA(inputFile, outputFile, e, n);
                cout << "Файл успешно зашифрован." << endl;
                break;
            }
            case 5: {
                cout << "Рекомендуется ввести полную пару ключей для проверки\n";
                cout << "Ввести только закрытый ключ (a) или полную пару с проверкой (m)? [a/m]: ";
                char keyType;
                cin >> keyType;
                cin.ignore();

                int64_t d, n, e;
                bool keysValid = false;

                if (keyType == 'm' || keyType == 'M') {
                    cout << "Введите полную пару ключей:\n";
                    if (getKeyPairManual(e, d, n)) {
                        keysValid = true;
                    }
                } else {
                    cout << "Введите ЗАКРЫТЫЙ ключ для дешифрования:\n";
                    if (getPrivateKeyManual(d, n)) {
                        keysValid = true;
                    }
                }

                if (!keysValid) {
                    cout << "Ошибка ввода ключей!\n";
                    break;
                }

                cout << "Введите имя файла для дешифрования: ";
                string inputFile;
                getline(cin, inputFile);

                cout << "Введите имя выходного файла: ";
                string outputFile;
                getline(cin, outputFile);

                decryptFileRSA(inputFile, outputFile, d, n);
                cout << "Файл успешно расшифрован." << endl;
                break;
            }
            default:
                cout << "Неверный выбор." << endl;
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}