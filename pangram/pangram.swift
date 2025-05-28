func isPangram(_ sentence: String) -> Bool {
    /**
     * Проверяет, является ли строка панграммой
     * @param sentence Входная строка
     * @return true если строка содержит все буквы алфавита, иначе false
     */
     
    let lowerSentence = sentence.lowercased()
    for c in "abcdefghijklmnopqrstuvwxyz" {
        if !lowerSentence.contains(c) {
            return false
        }
    }
    return true
}

print(isPangram("thequickbrownfoxjumpsoverthelazydog")) // true
print(isPangram("whatdoesthefoxsay")) // false