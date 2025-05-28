#include <iostream>
#include <cstdlib>

/**
 * Вычисляет сумму и произведение цифр числа, а также модуль их разности.
 * 
 * @param num Натуральное число для обработки
 */

void calculateDigitOps(int num) {
    int sum = 0, product = 1;
    int original = num;
    
    while (num > 0) {
        int digit = num % 10;
        sum += digit;
        product *= digit;
        num /= 10;
    }
    
    std::cout << sum << " " << product << " " << abs(sum - product) << std::endl;
}

int main() {
    int N;
    std::cin >> N;
    calculateDigitOps(N);
    return 0;
}