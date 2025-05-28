def main():
    """
    Определяет победителя в игре между Павлом и Викой.

    Входные данные:
        - n, m: целые числа (5 ≤ n ≤ 50000, 4 ≤ m ≤ 100).
        - Последовательность из n целых чисел.

    Выходные данные:
        - 1, если побеждает Павел; 0, если побеждает Вика.

    Алгоритм:
        - Использует динамическое программирование для вычисления оптимальной стратегии.
        - Префиксные суммы ускоряют расчёт сумм подмассивов.
        - dp[i] — максимальная разница очков, которую может получить игрок, начиная с позиции i.
    """
    import sys
    n, m = map(int, sys.stdin.readline().split())
    nums = list(map(int, sys.stdin.readline().split()))
    
    # Префиксные суммы
    prefix = [0] * (n + 1)
    for i in range(n):
        prefix[i+1] = prefix[i] + nums[i]
    
    # Инициализация массива dp
    dp = [-float('inf')] * (n + 1)
    dp[n] = 0  # База: если чисел нет, разница 0
    
    for i in range(n-1, -1, -1):
        max_k = min(m, n - i)
        for k in range(1, max_k + 1):
            current_sum = prefix[i + k] - prefix[i]
            dp[i] = max(dp[i], current_sum - dp[i + k])
    
    print(1 if dp[0] > 0 else 0)

if __name__ == "__main__":
    main()
    
