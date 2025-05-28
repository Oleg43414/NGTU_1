fun isPangram(sentence: String): Boolean {
    /**
     * Проверяет, является ли строка панграммой
     * @param sentence Входная строка
     * @return true если строка содержит все буквы алфавита, иначе false
     */
     
    return ('a'..'z').all { it in sentence.toLowerCase() }
}

fun main() {
    println(isPangram("thequickbrownfoxjumpsoverthelazydog")) // true
    println(isPangram("whatdoesthefoxsay")) // false
}