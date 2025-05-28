using System;

class DigitOps {
    /// <summary>
    /// Вычисляет сумму и произведение цифр числа, а также модуль их разности.
    /// </summary>
    /// <param name="num">Натуральное число для обработки</param>
    static void CalculateDigitOps(int num) {
        int sum = 0, product = 1;
        int original = num;
        
        while (num > 0) {
            int digit = num % 10;
            sum += digit;
            product *= digit;
            num /= 10;
        }
        
        Console.WriteLine($"{sum} {product} {Math.Abs(sum - product)}");
    }
    
    static void Main() {
        int N = Convert.ToInt32(Console.ReadLine());
        CalculateDigitOps(N);
    }
}