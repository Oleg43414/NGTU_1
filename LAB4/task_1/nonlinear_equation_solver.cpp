#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Функция уравнения: 2ln(x) - 0.5x + 1
double f(double x) {
    return 2 * log(x) - 0.5 * x + 1;
}

// Производная функции: 2/x - 0.5
double df(double x) {
    return 2 / x - 0.5;
}

// Метод половинного деления для нахождения обоих корней
void bisection(double a1, double b1, double a2, double b2, double eps) {
    cout << "\nМетод половинного деления:\n";
    
    // Поиск первого корня (0.7)
    cout << "Поиск первого корня (0.7):\n";
    cout << "N\ta\t\tb\t\tb - a\n";
    int n = 0;
    while (fabs(b1 - a1) > eps) {
        double c = (a1 + b1) / 2;
        cout << n << "\t" << fixed << setprecision(6) << a1 << "\t" << b1 << "\t" << b1 - a1 << endl;
        if (f(c) * f(a1) < 0) {
            b1 = c;
        } else {
            a1 = c;
        }
        n++;
    }
    cout << "Первый корень: " << (a1 + b1) / 2 << endl;
    
    // Поиск второго корня (11.9)
    cout << "\nПоиск второго корня (11.9):\n";
    cout << "N\ta\t\tb\t\tb - a\n";
    n = 0;
    while (fabs(b2 - a2) > eps) {
        double c = (a2 + b2) / 2;
        cout << n << "\t" << fixed << setprecision(6) << a2 << "\t" << b2 << "\t" << b2 - a2 << endl;
        if (f(c) * f(a2) < 0) {
            b2 = c;
        } else {
            a2 = c;
        }
        n++;
    }
    cout << "Второй корень: " << (a2 + b2) / 2 << endl;
}

// Метод Ньютона для нахождения обоих корней
void newton(double x0_1, double x0_2, double eps) {
    cout << "\nМетод Ньютона:\n";
    
    // Поиск первого корня (0.7)
    cout << "Поиск первого корня (0.7):\n";
    cout << "N\tXn\t\tXn+1\t\tXn+1 - Xn\n";
    double x = x0_1;
    int n = 0;
    while (true) {
        double fx = f(x);
        double dx = df(x);
        double x_next = x - fx / dx;
        cout << n << "\t" << fixed << setprecision(6) << x << "\t" << x_next << "\t" << fabs(x_next - x) << endl;
        if (fabs(x_next - x) < eps) break;
        x = x_next;
        n++;
    }
    cout << "Первый корень: " << x << endl;
    
    // Поиск второго корня (11.9)
    cout << "\nПоиск второго корня (11.9):\n";
    cout << "N\tXn\t\tXn+1\t\tXn+1 - Xn\n";
    x = x0_2;
    n = 0;
    while (true) {
        double fx = f(x);
        double dx = df(x);
        double x_next = x - fx / dx;
        cout << n << "\t" << fixed << setprecision(6) << x << "\t" << x_next << "\t" << fabs(x_next - x) << endl;
        if (fabs(x_next - x) < eps) break;
        x = x_next;
        n++;
    }
    cout << "Второй корень: " << x << endl;
}

// Метод простых итераций для нахождения обоих корней
void iteration(double x0_1, double x0_2, double eps) {
    cout << "\nМетод простых итераций:\n";
    
    // Поиск первого корня (0.7)
    cout << "Поиск первого корня (0.7):\n";
    cout << "N\tXn\t\tXn+1\t\tXn+1 - Xn\n";
    double x = x0_1;
    int n = 0;
    while (true) {
        double x_next = exp((0.5 * x - 1) / 2); // Преобразованное уравнение
        cout << n << "\t" << fixed << setprecision(6) << x << "\t" << x_next << "\t" << fabs(x_next - x) << endl;
        if (fabs(x_next - x) < eps) break;
        x = x_next;
        n++;
    }
    cout << "Первый корень: " << x << endl;
    
    // Поиск второго корня (11.9)
    cout << "\nПоиск второго корня (11.9):\n";
    cout << "N\tXn\t\tXn+1\t\tXn+1 - Xn\n";
    x = x0_2;
    n = 0;
    while (true) {
        double x_next = exp((0.5 * x - 1) / 2); // Преобразованное уравнение
        cout << n << "\t" << fixed << setprecision(6) << x << "\t" << x_next << "\t" << fabs(x_next - x) << endl;
        if (fabs(x_next - x) < eps) break;
        x = x_next;
        n++;
    }
    cout << "Второй корень: " << x << endl;
}

int main() {
    double eps = 1e-4;
    
    // Интервалы для метода половинного деления
    double a1 = 0.5, b1 = 1.0;    // Для первого корня (0.7)
    double a2 = 10.0, b2 = 12.0;  // Для второго корня (11.9)
    
    // Начальные приближения для методов Ньютона и итераций
    double x0_1 = 0.75;  
    double x0_2 = 11.5;  
    
    bisection(a1, b1, a2, b2, eps);
    newton(x0_1, x0_2, eps);
    iteration(x0_1, x0_2, eps);

    return 0;
}