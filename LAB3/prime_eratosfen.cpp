/**
 * @file prime_eratosfen.cpp
 * @brief Генерация и проверка простых чисел методами Эратосфена, Миллера, Поклингтона и ГОСТ
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <bitset>
#include <chrono>

using namespace std;

/**
 * @brief Генерирует список простых чисел до заданного предела методом решета Эратосфена
 * @return Вектор простых чисел меньше 500
 * @example
 * vector<int> primes = sieve_of_eratosthenes();
 * // primes = [2, 3, 5, 7, 11, ...]
 */

// 1. Решето Эратосфена для чисел <500
vector<int> sieve_of_eratosthenes() {
    const int limit = 500;
    vector<bool> sieve(limit + 1, true);
    sieve[0] = sieve[1] = false;
    
    for (int num = 2; num * num <= limit; ++num) {
        if (sieve[num]) {
            for (int multiple = num * num; multiple <= limit; multiple += num) {
                sieve[multiple] = false;
            }
        }
    }
    
    vector<int> primes;
    for (int num = 2; num <= limit; ++num) {
        if (sieve[num]) primes.push_back(num);
    }
    return primes;
}

/**
 * @brief Модульное возведение в степень (base^exponent % mod)
 * @param base Основание
 * @param exponent Показатель степени
 * @param mod Модуль
 * @return Результат вычисления base^exponent mod mod
 * @note Оптимизировано с использованием метода двоичного возведения в степень
 */

// 2a. Тест Миллера
uint64_t modular_pow(uint64_t base, uint64_t exponent, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exponent > 0) {
        if (exponent & 1) 
            result = (result * base) % mod;
        exponent >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

/**
 * @brief Тест Миллера на простоту числа
 * @param n Проверяемое число
 * @param factors Множители из разложения n-1
 * @param t Количество проверочных попыток (по умолчанию 5)
 * @return true если число вероятно простое, false если составное
 * @warning Требует предварительного разложения n-1 на множители
 */

bool miller_test(uint64_t n, const vector<int>& factors, int t = 5) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint64_t> dis(2, n-2);
    
    // Проверка a^(n-1) ≡ 1 mod n
    for (int i = 0; i < t; ++i) {
        uint64_t a = dis(gen);
        if (modular_pow(a, n-1, n) != 1)
            return false;
    }
    
    // Проверка a^((n-1)/q) ≡ 1 mod n
    for (int q : factors) {
        bool all_ones = true;
        for (int i = 0; i < t; ++i) {
            uint64_t a = dis(gen);
            if (modular_pow(a, (n-1)/q, n) == 1) {
                all_ones = false;
                break;
            }
        }
        if (all_ones) return false;
    }
    return true;
}

/**
 * @brief Генерация простого числа методом Миллера
 * @param target_bits Требуемая битовая длина простого числа
 * @param primes Таблица простых чисел для построения кандидата
 * @return Найденное простое число
 * @note Время работы зависит от удачного подбора параметров
 */

uint64_t generate_prime_miller(int target_bits, const vector<int>& primes) {
    random_device rd;
    mt19937 gen(rd());
    
    while (true) {
        // Генерация m
        uint64_t m = 1;
        vector<int> factors;
        while (m < (1ULL << (target_bits - 2))) {
            int idx = uniform_int_distribution<>(0, primes.size()-1)(gen);
            int q = primes[idx];
            int alpha = uniform_int_distribution<>(1, 2)(gen);
            m *= pow(q, alpha);
            factors.push_back(q);
        }
        
        uint64_t n = 2 * m + 1;
        if (miller_test(n, factors))
            return n;
    }
}

/**
 * @brief Генерация простого числа методом Поклингтона
 * @param target_bits Требуемая битовая длина простого числа
 * @param primes Таблица простых чисел для построения множителей
 * @return Найденное простое число
 * @throws Неявно может войти в бесконечный цикл при неудачных параметрах
 */

// 2б. Тест Поклингтона
uint64_t generate_prime_poklington(int target_bits, const vector<int>& primes) {
    random_device rd;
    mt19937 gen(rd());
    
    while (true) {
        // Генерация F и R
        uint64_t F = 1;
        vector<int> F_factors;
        while (F < (1ULL << (target_bits/2 - 1))) {
            int idx = uniform_int_distribution<>(0, primes.size()-1)(gen);
            int q = primes[idx];
            if (find(F_factors.begin(), F_factors.end(), q) == F_factors.end()) {
                F *= q;
                F_factors.push_back(q);
            }
        }
        
        uint64_t R = 1;
        while (true) {
            int idx = uniform_int_distribution<>(0, primes.size()-1)(gen);
            int q = primes[idx];
            if (find(F_factors.begin(), F_factors.end(), q) == F_factors.end()) {
                R *= q;
                if ((F * R + 1) >= (1ULL << (target_bits - 1)))
                    break;
            }
        }
        
        uint64_t n = F * R + 1;
        // Проверка Поклингтона
        uniform_int_distribution<uint64_t> dis(2, n-2);
        uint64_t a = dis(gen);
        if (modular_pow(a, n-1, n) != 1) continue;
        
        bool valid = true;
        for (int q : F_factors) {
            if (gcd(modular_pow(a, (n-1)/q, n) - 1, n) != 1) {
                valid = false;
                break;
            }
        }
        if (valid) return n;
    }
}

/**
 * @brief Тест Миллера-Рабина для проверки простоты
 * @param n Проверяемое число
 * @param k Количество раундов проверки (по умолчанию 5)
 * @return true если число вероятно простое, false если составное
 * @note Соответствует требованиям ГОСТ Р 34.10-94
 */

// 2в. ГОСТ Р 34.10-94 (Миниатюрная реализация)
bool miller_rabin(uint64_t n, int k = 5) {
    if (n < 2) return false;
    for (int p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        if (n % p == 0) return n == p;
    }
    
    // Разложение n-1 = d*2^s
    uint64_t d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint64_t> dis(2, n-2);
    
    for (int i = 0; i < k; ++i) {
        uint64_t a = dis(gen);
        uint64_t x = modular_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        
        bool composite = true;
        for (int j = 0; j < s-1; ++j) {
            x = modular_pow(x, 2, n);
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/**
 * @brief Генерация простого числа по ГОСТ Р 34.10-94
 * @param target_bits Требуемая битовая длина простого числа
 * @return Сгенерированное простое число
 * @warning Для больших чисел (>64 бит) требуется модификация
 */

uint64_t gost_generate_prime(int target_bits) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(
        1ULL << (target_bits - 1),
        (1ULL << target_bits) - 1
    );
    
    while (true) {
        uint64_t candidate = dis(gen);
        candidate |= 1; // Делаем нечетным
        if (miller_rabin(candidate))
            return candidate;
    }
}

int main() {
    auto primes = sieve_of_eratosthenes();
    cout << "First 10 primes under 500: ";
    for (int i = 0; i < 10; ++i) cout << primes[i] << " ";
    cout << "\n\n";
    
    cout << "Miller test prime (16-bit): " 
         << generate_prime_miller(16, primes) << endl;
         
    cout << "Poklington test prime (16-bit): " 
         << generate_prime_poklington(16, primes) << endl;
         
    cout << "GOST prime (16-bit): " 
         << gost_generate_prime(16) << endl;
    
    return 0;
}
