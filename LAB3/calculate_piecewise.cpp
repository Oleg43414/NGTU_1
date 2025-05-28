/**
 * @file piecewise_function.cpp
 * @brief Вычисление значений кусочно-заданной функции и вывод таблицы.
 * 
 * Программа вычисляет значения функции, заданной графически, на интервале [-4, 8] 
 * с шагом 0.5. Функция имеет следующие ветви:
 * - Не определена при x < -3 и x > 7.
 * - Линейные участки с коэффициентом 1 (45°).
 * - Постоянные значения на отдельных интервалах.
 * 
 * Формат вывода: таблица с колонками x и y. Значения округляются до 0.1.
 * Для неопределенных значений выводится "не определено".
 */

/**
 * doxygen -g
 * doxygen Doxyfile
 */
 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

double calculateFunction(double x) {
    // Определяем кусочно-заданную функцию на основе предположений о графике
    if (x < -3) {
        // Линейный участок слева от -3 (предположение)
        return -1;
    } else if (x >= -3 && x < -1) {
        // Линейный участок между -3 и -1 (предположение)
        return ((x + 7)/2);
    } else if (x >= -1 && x < 1) {
        // Участок под 45 градусами (y = x)
        return x;
    } else if (x >= 1 && x < 5) {
        // Горизонтальный участок (предположение)
        return 1;
    } else if (x >= 5 && x <= 7) {
        // Линейный участок между 5 и 7 (предположение)
        return -0.5 * x + 3.5;
    } else {
        // Значение по умолчанию для x > 7
        return -1;
    }
}

void printTableToConsole(double xStart, double xEnd, double dx) {
    cout << "-----------------------------" << "\n" << "|      x      |     f(x)    |" << "\n" << "-----------------------------" << endl;
    
    for (double x = xStart; x <= xEnd + 1e-9; x += dx) {
        double y = calculateFunction(x);
        cout << "| " << setw(11) << x << " | " << setw(11) << y << " |" << endl;
    }
    
    cout << "-----------------------------" << endl;
}

void printTableToFile(double xStart, double xEnd, double dx, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        return;
    }
    
    outFile << "-----------------------------" << endl;
    outFile << "|      x      |     f(x)    |" << endl;
    outFile << "-----------------------------" << endl;
    
    for (double x = xStart; x <= xEnd + 1e-9; x += dx) {
        double y = calculateFunction(x);
        outFile << "| " << setw(11) << x << " | " << setw(11) << y << " |" << endl;
    }
    
    outFile << "-----------------------------" << endl;
    outFile.close();
}

int main() {
    // Устанавливаем параметры для проверки всех ветвей функции
    double xStart = -4.0;  // Начало интервала
    double xEnd = 8.0;     // Конец интервала
    double dx = 0.5;       // Шаг
    
    // Выводим таблицу в консоль
    printTableToConsole(xStart, xEnd, dx);
    
    // Дополнительно сохраняем в файл
    printTableToFile(xStart, xEnd, dx, "function_table.txt");
    
    cout << "Таблица также сохранена в файл function_table.txt" << endl;
    
    return 0;
}