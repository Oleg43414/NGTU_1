using System;
using System.Linq;

class Program
{
    /// <summary>
    /// Проверяет, является ли строка панграммой (содержит все буквы английского алфавита)
    /// </summary>
    /// <param name="sentence">Входная строка для проверки</param>
    /// <returns>True если строка является панграммой, иначе False</returns>
    public static bool IsPangram(string sentence)
    {
        if (string.IsNullOrEmpty(sentence))
            return false;

        const string alphabet = "abcdefghijklmnopqrstuvwxyz";
        var lowerSentence = sentence.ToLowerInvariant();
        return alphabet.All(c => lowerSentence.Contains(c));
    }

    static void Main(string[] args)
    {
        Console.WriteLine("Проверка панграмм:");
        Console.WriteLine("==================");
        
        TestPangram("The quick brown fox jumps over the lazy dog", true);
        TestPangram("Pack my box with five dozen liquor jugs", true);
        TestPangram("What does the fox say", false);
        TestPangram("", false);
        TestPangram("ABCDEFGHIJKLMNOPQRSTUVWXYZ", true);
    }

    private static void TestPangram(string sentence, bool expected)
    {
        bool result = IsPangram(sentence);
        Console.WriteLine($"\"{sentence}\" -> {result} (Ожидается: {expected})");
    }
}
