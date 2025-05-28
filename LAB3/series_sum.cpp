/**
 * @file series_sum.cpp
 * @brief Вычисление суммы бесконечного ряда вида ∑(n^a / b^n).
 * 
 * Программа принимает два целых числа a и b (1 <= a, b <= 10).
 * Если ряд расходится (b = 1), выводится "infinity".
 * Если сумма рациональна, выводится несократимая дробь.
 * Для a > 2 вывод "irrational" (требуется расширение формул).
 * 
 * Примеры:
 * Ввод: 1 2 → Вывод: 2/1
 * Ввод: 2 1 → Вывод: infinity
 */

 #include <iostream>
 #include <algorithm> 
 
 using namespace std;
 
 /**
  * @brief Вычисляет наибольший общий делитель (НОД) двух целых чисел.
  * @param a Первое число.
  * @param b Второе число.
  * @return НОД(a, b).
  */
 int gcd(int a, int b) {
     while (b != 0) {
         int temp = b;
         b = a % b;
         a = temp;
     }
     return a;
 }
 
 /**
  * @brief Основная функция программы.
  * 
  * Считывает входные данные, проверяет условия сходимости, 
  * вычисляет сумму ряда и выводит результат.
  */
 int main() {
     int a, b; ///< Входные числа: a и b (1 <= a, b <= 10)
     cin >> a >> b;
 
     if (b == 1) {
         cout << "infinity" << endl;
         return 0;
     }
 
     int numerator;   ///< Числитель дроби суммы ряда
     int denominator; ///< Знаменатель дроби суммы ряда
 
     // Вычисление числителя и знаменателя в зависимости от a
     switch (a) {
         case 1:
             numerator = b;
             denominator = (b - 1) * (b - 1);
             break;
         case 2:
             numerator = b * (b + 1);
             denominator = (b - 1) * (b - 1) * (b - 1);
             break;
         default:
             // Для a > 2 требуется реализация дополнительных формул
             cout << "irrational" << endl;
             return 0;
     }
 
     // Сокращение дроби
     int common_divisor = gcd(numerator, denominator);
     numerator /= common_divisor;
     denominator /= common_divisor;
 
     cout << numerator << "/" << denominator << endl;
 
     return 0;
 }
 