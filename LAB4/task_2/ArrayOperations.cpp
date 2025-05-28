/**
 * @file
 * @brief Решение задания по работе с массивами:
 * 1. Генерация массива из n целых чисел (n >= 10) в диапазоне [-100, 100].
 * 2. Вычисление средних арифметических положительных и отрицательных элементов.
 *    Обмен местами максимального и минимального элементов.
 * 3. Поиск самой длинной убывающей последовательности.
 * 4. Вывод массива N×5, начиная с 1000.
 */

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

using namespace std;

/**
 * Генерирует массив случайных целых чисел в диапазоне [-100, 100].
 * @param n Размер массива (n >= 10).
 * @return Сгенерированный массив.
 */
vector<double> generateArray(double n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(-100, 100);
    vector<double> arr;
    for (int i = 0; i < n; ++i) {
        arr.push_back(distr(gen));
    }
    return arr;
}

/**
 * Находит средние арифметические положительных и отрицательных элементов.
 * Меняет местами максимальный и минимальный элементы.
 * @param arr Исходный массив.
 * @return Кортеж: (среднее положительных, среднее отрицательных, изменённый массив).
 */
tuple<double, double, vector<double>> processArray(vector<double> arr) {
    double sum_pos = 0, count_pos = 0;
    double sum_neg = 0, count_neg = 0;
    
    for (int num : arr) {
        if (num > 0) {
            sum_pos += num;
            count_pos++;
        } else if (num < 0) {
            sum_neg += num;
            count_neg++;
        }
    }
    
    double avg_pos = count_pos > 0 ? (double)sum_pos / count_pos : 0.0;
    double avg_neg = count_neg > 0 ? (double)sum_neg / count_neg : 0.0;
    
    auto [min_it, max_it] = minmax_element(arr.begin(), arr.end());
    iter_swap(min_it, max_it);
    
    return make_tuple(avg_pos, avg_neg, arr);
}

/**
 * Находит самую длинную убывающую последовательность в массиве.
 * @param arr Исходный массив.
 * @return Вектор, содер последовательность.
 */
vector<double> findDecreasingSequence(const vector<double>& arr) {
    vector<double> current = {arr[0]};
    vector<double> longest;
    
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < current.back()) {
            current.push_back(arr[i]);
        } else {
            if (current.size() > longest.size()) {
                longest = current;
            }
            current = {arr[i]};
        }
    }
    
    if (current.size() > longest.size()) {
        longest = current;
    }
    
    return longest;
}

/**
 * Выводит массив в формате N×5, начиная с 1000.
 * @param N Количество строк.
 */
void printFormattedArray(double N) {
    double start = 1000;
    for (double i = 0; i < N; ++i) {
        for (double j = 0; j < 5; ++j) {
            cout << start + i * 5 + j << " ";
        }
        cout << endl;
    }
}

int main() {
    // Пункт 1
    auto arr = generateArray(10);
    cout << "1. Исходный массив: ";
    for (double num : arr) cout << num << " ";
    cout << endl;

    // Пункт 2
    auto [avg_pos, avg_neg, processed_arr] = processArray(arr);
    cout << "\n2. Среднее положительных: " << avg_pos << endl;
    cout << "Среднее отрицательных: " << avg_neg << endl;
    cout << "Массив после замены: ";
    for (double num : processed_arr) cout << num << " ";
    cout << endl;

    // Пункт 3
    auto seq = findDecreasingSequence(processed_arr);
    cout << "\n3. Убывающая последовательность: ";
    for (double num : seq) cout << num << " ";
    cout << endl;

    // Пункт 4
    cout << "\n4. Форматированный массив (N=4):" << endl;
    printFormattedArray(4);

    return 0;
}
