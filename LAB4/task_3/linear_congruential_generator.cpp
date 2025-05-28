#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

/**
 * @brief Генерирует последовательность чисел с помощью линейного конгруэнтного генератора и находит индекс начала цикла.
 * @param X0 Начальное значение (seed).
 * @param A Множитель.
 * @param B Инкремент.
 * @param C Модуль.
 */
void generateLCG(int X0, int A, int B, int C) {
    unordered_map<int, int> seen; // Хранит число и его индекс
    vector<int> sequence;         // Последовательность чисел
    int current = X0;
    int index = 0;

    // Генерация чисел до обнаружения повтора
    while (true) {
        if (seen.find(current) != seen.end()) {
            // Вывод последовательности
            for (int num : sequence) {
                cout << num << " ";
            }
            // Вывод индекса начала цикла
            cout << "\n" << seen[current] << endl;
            break;
        }
        seen[current] = index;
        sequence.push_back(current);
        current = (static_cast<long long>(A) * current + B) % C;
        index++;
    }
}

int main() {
    int X0, A, B, C;
    // Ввод параметров
    cin >> X0 >> A >> B >> C;
    generateLCG(X0, A, B, C);
    return 0;
}
