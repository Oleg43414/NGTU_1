/*
    [ Сборка и запуск ]
    g++ -std=c++17 -Wall -Wextra unvisited_stones.cpp -o unvisited_stones
    ./unvisited_stones
*/

#include <vector>
#include <algorithm>
#include <iostream>

/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param N Количество птиц.
 * @param stones Общее количество валунов.
 * @param birds Массив с дистанциями перелёта каждой птицы.
 * @return Количество непосещённых валунов.
 */

int unvisitedStones(int N, int stones, std::vector<int>& birds) {
    std::vector<bool> visited(stones + 1, false);
    for (int bird : birds) {
        for (int pos = bird; pos <= stones; pos += bird) {
            visited[pos] = true;
        }
    }
    return std::count(visited.begin() + 1, visited.end(), false);
}

int main() {
    // Пример использования
    int N = 3;                      // Количество птиц
    int stones = 10;                // Общее количество валунов
    std::vector<int> birds = {2, 3, 5}; // Дистанции перелёта птиц

    int result = unvisitedStones(N, stones, birds);
    std::cout << "Непосещённых валунов: " << result << std::endl;

    return 0;
}
