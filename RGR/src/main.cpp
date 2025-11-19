#include <iostream>
#include <dlfcn.h>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Простые объявления структур (без лишних деталей)
struct RSAKeys {
    int64_t publicKey;
    int64_t privateKey;
    int64_t n;
};

struct ThreeWayKeys {
    uint32_t key[3];
};

// Упрощенные типы для функций
typedef void (*RunRSACryptoFunc)();
typedef void (*RunThreeWayCryptoFunc)();
typedef void (*RunMorseDemoFunc)();

// Функция для скрытого ввода пароля
string get_password(const string& prompt = "Введите пароль: ") {
    struct termios oldt, newt;
    string password;
    char c;
    
    cout << prompt;
    cout.flush();
    
    // Сохраняем текущие настройки терминала
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    // Отключаем отображение вводимых символов
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Читаем пароль посимвольно
    while (read(STDIN_FILENO, &c, 1) == 1 && c != '\n' && c != '\r') {
        if (c == 127 || c == 8) { // Backspace
            if (!password.empty()) {
                password.pop_back();
            }
        } else {
            password += c;
        }
    }
    
    // Восстанавливаем настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    cout << endl; // Переходим на новую строку после ввода пароля
    return password;
}

// Функция для хеширования пароля (простая реализация)
size_t simple_hash(const string& password) {
    size_t hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
    }
    return hash;
}

// Функция аутентификации
bool authenticate() {
    const string stored_password_hash = "118311674"; // Хеш пароля "crypto123"
    int attempts = 3;
    
    cout << "=== СИСТЕМА АУТЕНТИФИКАЦИИ ===" << endl;
    cout << "Добро пожаловать в криптографическую систему!" << endl;
    
    while (attempts > 0) {
        string password = get_password();
        size_t input_hash = simple_hash(password);
        
        // Отладочная информация (можно удалить после тестирования)
        cout << "Введенный пароль: " << password << endl;
        cout << "Хеш введенного пароля: " << input_hash << endl;
        cout << "Ожидаемый хеш: " << stored_password_hash << endl;
        
        if (to_string(input_hash) == stored_password_hash) {
            cout << "✓ Аутентификация успешна!" << endl;
            cout << "Доступ к криптографическим модулям разрешен." << endl;
            return true;
        } else {
            attempts--;
            if (attempts > 0) {
                cerr << "✗ Неверный пароль. Осталось попыток: " << attempts << endl;
            } else {
                cerr << "✗ Превышено количество попыток. Доступ запрещен." << endl;
            }
        }
    }
    
    return false;
}

// Альтернативная упрощенная аутентификация (без хеширования)
bool simple_authenticate() {
    const string correct_password = "NGTU";
    int attempts = 3;
    
    cout << "=== СИСТЕМА АУТЕНТИФИКАЦИИ ===" << endl;
    cout << "Добро пожаловать в криптографическую систему!" << endl;
    
    while (attempts > 0) {
        string password = get_password();
        
        if (password == correct_password) {
            cout << "✓ Аутентификация успешна!" << endl;
            cout << "Доступ к криптографическим модулям разрешен." << endl;
            return true;
        } else {
            attempts--;
            if (attempts > 0) {
                cerr << "✗ Неверный пароль. Осталось попыток: " << attempts << endl;
            } else {
                cerr << "✗ Превышено количество попыток. Доступ запрещен." << endl;
            }
        }
    }
    
    return false;
}


void show_menu() {
    cout << "\n=== КРИПТОГРАФИЧЕСКАЯ СИСТЕМА & АЗБУКА МОРЗЕ ===" << endl;
    cout << "1. Запустить RSA интерактивный режим" << endl;
    cout << "2. Запустить 3-WAY интерактивный режим" << endl;
    cout << "3. Запустить Азбуку Морзе" << endl;
    cout << "4. Выход" << endl;
    cout << "Выберите действие: ";
}

void show_welcome() {
    cout << "==========================================" << endl;
    cout << "    КРИПТОГРАФИЧЕСКАЯ СИСТЕМА" << endl;
    cout << "    Реализация RSA, 3-WAY и Азбуки Морзе" << endl;
    cout << "==========================================" << endl;
}

int main() {
    show_welcome();
    
    // Аутентификация пользователя - используем упрощенную версию
    if (!simple_authenticate()) {
        cerr << "Программа завершена." << endl;
        return 1;
    }
    
    void* rsa_handle = nullptr;
    void* threeway_handle = nullptr;
    void* morse_handle = nullptr;
    char* error = nullptr;
    
    // Загружаем RSA библиотеку
    rsa_handle = dlopen("./lib/librsa.so", RTLD_LAZY);
    if (!rsa_handle) {
        cerr << "Ошибка загрузки RSA библиотеки: " << dlerror() << endl;
        cerr << "Убедитесь, что файл librsa.so находится в директории ./lib/" << endl;
    } else {
        cout << "✓ RSA библиотека успешно загружена!" << endl;
    }
    
    // Загружаем 3-WAY библиотеку
    threeway_handle = dlopen("./lib/libthreeway.so", RTLD_LAZY);
    if (!threeway_handle) {
        cerr << "Ошибка загрузки 3-WAY библиотеки: " << dlerror() << endl;
        cerr << "Убедитесь, что файл libthreeway.so находится в директории ./lib/" << endl;
    } else {
        cout << "✓ 3-WAY библиотека успешно загружена!" << endl;
    }
    
    // Загружаем Morse библиотеку
    morse_handle = dlopen("./lib/libmorse.so", RTLD_LAZY);
    if (!morse_handle) {
        cerr << "Ошибка загрузки Morse библиотеки: " << dlerror() << endl;
        cerr << "Убедитесь, что файл libmorse.so находится в директории ./lib/" << endl;
    } else {
        cout << "✓ Morse библиотека успешно загружена!" << endl;
    }
    
    // Сбрасываем ошибки
    dlerror();
    
    // Загружаем RSA функцию (если библиотека загружена)
    RunRSACryptoFunc runRSACrypto = nullptr;
    if (rsa_handle) {
        runRSACrypto = (RunRSACryptoFunc)dlsym(rsa_handle, "run_rsa_crypto");
        if ((error = dlerror()) != nullptr) {
            cerr << "Ошибка загрузки RSA функции: " << error << endl;
            runRSACrypto = nullptr;
        }
    }
    
    // Сбрасываем ошибки
    dlerror();
    
    // Загружаем 3-WAY функцию (если библиотека загружена)
    RunThreeWayCryptoFunc runThreeWayCrypto = nullptr;
    if (threeway_handle) {
        runThreeWayCrypto = (RunThreeWayCryptoFunc)dlsym(threeway_handle, "run_threeway_crypto");
        if ((error = dlerror()) != nullptr) {
            cerr << "Ошибка загрузки 3-WAY функции: " << error << endl;
            runThreeWayCrypto = nullptr;
        }
    }
    
    // Сбрасываем ошибки
    dlerror();
    
    // Загружаем Morse функцию (если библиотека загружена)
    RunMorseDemoFunc runMorseDemo = nullptr;
    if (morse_handle) {
        runMorseDemo = (RunMorseDemoFunc)dlsym(morse_handle, "run_morse_demo");
        if ((error = dlerror()) != nullptr) {
            cerr << "Ошибка загрузки Morse функции: " << error << endl;
            runMorseDemo = nullptr;
        }
    }
    
    cout << "✓ Система инициализирована!" << endl;
    
    int choice;
    do {
        show_menu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                if (runRSACrypto) {
                    cout << "\n=== Запуск RSA системы ===" << endl;
                    runRSACrypto();
                } else {
                    cerr << "✗ RSA функция не загружена!" << endl;
                    cerr << "Доступные библиотеки в ./lib/:" << endl;
                    system("ls -la ./lib/ 2>/dev/null || echo 'Директория ./lib/ не существует'");
                }
                break;
                
            case 2:
                if (runThreeWayCrypto) {
                    cout << "\n=== Запуск 3-WAY системы ===" << endl;
                    runThreeWayCrypto();
                } else {
                    cerr << "✗ 3-WAY функция не загружена!" << endl;
                    cerr << "Доступные библиотеки в ./lib/:" << endl;
                    system("ls -la ./lib/ 2>/dev/null || echo 'Директория ./lib/ не существует'");
                }
                break;
                
            case 3:
                if (runMorseDemo) {
                    cout << "\n=== Запуск Азбуки Морзе ===" << endl;
                    runMorseDemo();
                } else {
                    cerr << "✗ Morse функция не загружена!" << endl;
                    cerr << "Доступные библиотеки в ./lib/:" << endl;
                    system("ls -la ./lib/ 2>/dev/null || echo 'Директория ./lib/ не существует'");
                }
                break;
            case 4:
                cout << "Выход из программы." << endl;
                break;
                
            default:
                cout << "Неверный выбор! Пожалуйста, выберите от 1 до 5." << endl;
        }
        
        if (choice != 4) {
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
        }
        
    } while (choice != 4);
    
    // Закрываем библиотеки
    if (rsa_handle) {
        dlclose(rsa_handle);
        cout << "✓ RSA библиотека выгружена." << endl;
    }
    if (threeway_handle) {
        dlclose(threeway_handle);
        cout << "✓ 3-WAY библиотека выгружена." << endl;
    }
    if (morse_handle) {
        dlclose(morse_handle);
        cout << "✓ Morse библиотека выгружена." << endl;
    }
    
    cout << "Сеанс работы завершен." << endl;
    return 0;
}