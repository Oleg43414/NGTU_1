#include <stdio.h>

// Объявление ассемблерной функции
int unvisited_stones(int N, int stones, int* birds);

int main() {
    int N = 3;                     // Количество птиц
    int stones = 10;               // Всего валунов
    int birds[] = {2, 3, 5};       // Дистанции птиц

    int result = unvisited_stones(N, stones, birds);
    printf("Unvisited stones: %d\n", result);

    return 0;
}