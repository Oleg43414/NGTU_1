#include <iostream>
#include <vector>
#include <climits>

using namespace std;

/**
 * @file game_winner.cpp
 * @brief Определяет победителя в игре между Павлом и Викой.
 * 
 * Входные данные:
 * - n, m: целые числа (5 ≤ n ≤ 50000, 4 ≤ m ≤ 100).
 * - Последовательность из n целых чисел.
 * 
 * Выходные данные:
 * - 1, если побеждает Павел; 0, если побеждает Вика.
 * 
 * Алгоритм:
 * - Использует динамическое программирование для вычисления оптимальной стратегии.
 * - Префиксные суммы ускоряют расчёт сумм подмассивов.
 * - dp[i] — максимальная разница очков, которую может получить игрок, начиная с позиции i.
 */

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    
    // Префиксные суммы
    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i+1] = prefix[i] + nums[i];
    }
    
    // Массив dp
    vector<long long> dp(n + 1, LLONG_MIN);
    dp[n] = 0; // База
    
    for (int i = n-1; i >= 0; --i) {
        int max_k = min(m, n - i);
        for (int k = 1; k <= max_k; ++k) {
            long long current_sum = prefix[i + k] - prefix[i];
            dp[i] = max(dp[i], current_sum - dp[i + k]);
        }
    }
    
    cout << (dp[0] > 0 ? 1 : 0) << endl;
    return 0;
}
