using System;
using System.Linq;

class PangramChecker
{
    /**
     * Проверяет, является ли строка панграммой
     * @param sentence Входная строка
     * @return true если строка содержит все буквы алфавита, иначе false
     */
     
    public static bool IsPangram(string sentence)
    {
        return "abcdefghijklmnopqrstuvwxyz".All(c => sentence.ToLower().Contains(c));
    }

    static void Main()
    {
        Console.WriteLine(IsPangram("thequickbrownfoxjumpsoverthelazydog")); // True
        Console.WriteLine(IsPangram("whatdoesthefoxsay")); // False
    }
}