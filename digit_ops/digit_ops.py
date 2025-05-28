def calculate_digit_ops(num):
    """
    Вычисляет сумму и произведение цифр числа, а также модуль их разности.
    
    Аргументы:
        num (int): Натуральное число для обработки
    
    Возвращает:
        None: Выводит результат в формате "sum product abs_diff"
    """
    sum_digits = 0
    product = 1
    n = num
    
    while n > 0:
        digit = n % 10
        sum_digits += digit
        product *= digit
        n = n // 10
    
    print(f"{sum_digits} {product} {abs(sum_digits - product)}")

N = int(input())
calculate_digit_ops(N)