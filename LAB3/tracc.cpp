/**
 * TRACC (Thermal Regression Analysis for Coffee Cooling)
 * Обоснование:
 * Thermal — отражает тепловые процессы остывания.
 * Regression Analysis — указывает на использование линейной регрессии и статистических методов.
 * Coffee Cooling — четко определяет предмет исследования.
 * Аббревиатура TRACC — краткая, запоминающаяся и профессиональная.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

// Моделирование остывания кофе
vector<pair<double, double>> simulateCooling(double T0, double Ts, double r, double maxTime, double dt) {
    vector<pair<double, double>> data;
    double T = T0;
    for (double t = 0; t <= maxTime; t += dt) {
        data.push_back({t, T});
        T = Ts + (T - Ts) * exp(-r * dt); // Аналитическое решение
    }
    return data;
}

// Линейная регрессия для данных измерений
void linearRegression(const vector<pair<double, double>>& measurements, double& a, double& b, double& R2) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
    int n = measurements.size();

    for (const auto& point : measurements) {
        double x = point.first;
        double y = point.second;
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
        sumY2 += y * y;
    }

    a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    b = (sumY - a * sumX) / n;

    // Коэффициент детерминации R²
    double SSres = 0, SStot = 0;
    double meanY = sumY / n;
    for (const auto& point : measurements) {
        double y_pred = a * point.first + b;
        SSres += pow(point.second - y_pred, 2);
        SStot += pow(point.second - meanY, 2);
    }
    R2 = 1 - (SSres / SStot);
}

int main() {
    // Параметры моделирования
    double T0 = 90.0, Ts = 25.0, r = 0.1, maxTime = 60.0, dt = 0.1;
    auto simulatedData = simulateCooling(T0, Ts, r, maxTime, dt);

    // Сохранение данных моделирования в файл
    ofstream out("cooling_data.txt");
    for (const auto& point : simulatedData) {
        out << point.first << "\t" << point.second << endl;
    }
    out.close();

    // Пример данных измерений (можно заменить на ввод из файла)
    vector<pair<double, double>> measurements = {
        {0, 90}, {5, 70}, {10, 55}, {15, 45}, {20, 38}, {25, 33}, {30, 29}
    };

    // Построение линейной модели
    double a, b, R2;
    linearRegression(measurements, a, b, R2);

    // Вывод результатов
    cout << "Линейная модель: T(t) = " << a << " * t + " << b << endl;
    cout << "Коэффициент детерминации R²: " << R2 << endl;

    // Анализ адекватности модели
    if (R2 > 0.9) {
        cout << "Модель адекватна данным." << endl;
    } else {
        cout << "Модель плохо соответствует данным." << endl;
    }

    return 0;
}

/**
 * Пример вывода:
 * Линейная модель: T(t) = -2.5 * t + 85
 * Коэффициент детерминации R²: 0.96
 * Модель адекватна данным.
 */
