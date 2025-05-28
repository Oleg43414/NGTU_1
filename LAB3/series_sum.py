import math

def gcd(a: int, b: int) -> int:
    """
    Вычисляет наибольший общий делитель (НОД) двух целых чисел.

    Параметры:
        a (int): Первое число.
        b (int): Второе число.

    Возвращает:
        int: Наибольший общий делитель чисел a и b.
    """
    while b:
        a, b = b, a % b
    return a

# Считывание входных данных
a, b = map(int, input().split())

if b == 1:
    print("infinity")
else:
    """
    Вычисляет сумму ряда ∑(n^a / b^n) для заданных a и b.
    
    Условия:
        - 1 <= a, b <= 10
        - При b = 1 ряд расходится (вывод 'infinity').
        - Для a > 2 требуется расширение формул (временная заглушка: 1/1).

    Формулы:
        - a = 1: S = b / (b-1)^2
        - a = 2: S = b(b+1) / (b-1)^3
    """
    if a == 1:
        numerator = b
        denominator = (b - 1) ** 2
    elif a == 2:
        numerator = b * (b + 1)
        denominator = (b - 1) ** 3
    else:
        # Заглушка для a > 2 (требуется реализация)
        numerator = 1
        denominator = 1

    # Сокращение дроби
    common_divisor = gcd(numerator, denominator)
    numerator //= common_divisor
    denominator //= common_divisor

    print(f"{numerator}/{denominator}")
    