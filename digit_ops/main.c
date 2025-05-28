#include <stdio.h>

// Объявляем ассемблерную функцию
extern int digit_ops(int number);

int main() {
    int number;
    printf("Enter number: ");
    scanf("%d", &number);

    digit_ops(number);  // Вызываем ассемблерную функцию
    return 0;
}
