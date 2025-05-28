public class PangramChecker {
    /**
     * Проверяет, является ли строка панграммой
     * @param sentence Входная строка
     * @return true если строка содержит все буквы алфавита, иначе false
     */
    
    public static boolean isPangram(String sentence) {
        String lowerSentence = sentence.toLowerCase();
        for (char c = 'a'; c <= 'z'; c++) {
            if (lowerSentence.indexOf(c) == -1) {
                return false;
            }
        }
        return true;
    }

    public static void main(String[] args) {
        System.out.println(isPangram("thequickbrownfoxjumpsoverthelazydog")); // true
        System.out.println(isPangram("whatdoesthefoxsay")); // false
    }
}