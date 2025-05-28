using System;
using System.Linq;

public class Solution 
{
    /// <summary>
    /// Находит количество валунов, которые не посетит ни одна птица
    /// </summary>
    /// <param name="birdsCount">Количество птиц</param>
    /// <param name="stonesCount">Общее количество валунов</param>
    /// <param name="birdsSteps">Массив с дистанциями перелёта каждой птицы</param>
    /// <returns>Количество непосещённых валунов</returns>
    public int GetUnvisitedStonesCount(int birdsCount, int stonesCount, int[] birdsSteps)
    {
        if (birdsSteps == null)
            throw new ArgumentNullException(nameof(birdsSteps));
            
        if (birdsCount != birdsSteps.Length)
            throw new ArgumentException("Количество птиц не совпадает с длиной массива шагов");
        
        var visitedStones = new bool[stonesCount + 1]; // Индексы 1..stonesCount
        
        foreach (var step in birdsSteps)
        {
            if (step <= 0)
                throw new ArgumentException("Шаг птицы должен быть положительным числом");
                
            for (int stone = step; stone <= stonesCount; stone += step)
            {
                visitedStones[stone] = true;
            }
        }
        
        return visitedStones.Count(stoneVisited => !stoneVisited) - 1; // -1 потому что 0-й элемент не учитывается
    }
}

// Пример использования
public class Program
{
    public static void Main()
    {
        var solution = new Solution();
        
        // Тест 1
        int result1 = solution.GetUnvisitedStonesCount(2, 6, new int[] {3, 2});
        Console.WriteLine($"Тест 1: {result1}"); // Ожидаем 2
        
        // Тест 2
        int result2 = solution.GetUnvisitedStonesCount(1, 10, new int[] {2});
        Console.WriteLine($"Тест 2: {result2}"); // Ожидаем 5
        
        // Тест 3
        int result3 = solution.GetUnvisitedStonesCount(3, 10, new int[] {1, 2, 3});
        Console.WriteLine($"Тест 3: {result3}"); // Ожидаем 0
    }
}
