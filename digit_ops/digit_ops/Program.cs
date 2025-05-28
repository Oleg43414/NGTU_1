using System;

class Program
{
    /// <summary>
    /// Вычисляет сумму и произведение цифр числа, а также модуль их разности
    /// </summary>
    /// <param name="number">Натуральное число для обработки</param>
    /// <returns>Кортеж (sum, product, absoluteDifference)</returns>
    public static (int sum, int product, int absoluteDifference) CalculateDigitOperations(int number)
    {
        if (number <= 0)
            throw new ArgumentException("Число должно быть натуральным (положительным целым)");

        int sum = 0;
        int product = 1;
        int tempNumber = number;

        while (tempNumber > 0)
        {
            int digit = tempNumber % 10;
            sum += digit;
            product *= digit;
            tempNumber /= 10;
        }

        return (sum, product, Math.Abs(sum - product));
    }

    static void Main(string[] args)
    {
        try
        {
            Console.WriteLine("Введите натуральное число:");
            string input = Console.ReadLine();
            
            if (int.TryParse(input, out int number) && number > 0)
            {
                var result = CalculateDigitOperations(number);
                Console.WriteLine($"Сумма цифр: {result.sum}");
                Console.WriteLine($"Произведение цифр: {result.product}");
                Console.WriteLine($"Модуль разности: {result.absoluteDifference}");
            }
            else
            {
                Console.WriteLine("Ошибка: Введите корректное натуральное число.");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Произошла ошибка: {ex.Message}");
        }
    }
}
