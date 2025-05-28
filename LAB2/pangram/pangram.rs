fn is_pangram(sentence: &str) -> bool {
    /**
     * Проверяет, является ли строка панграммой
     * @param sentence Входная строка
     * @return true если строка содержит все буквы алфавита, иначе false
     */
     
    let lower_sentence = sentence.to_lowercase();
    ('a'..='z').all(|c| lower_sentence.contains(c))
}

fn main() {
    println!("{}", is_pangram("thequickbrownfoxjumpsoverthelazydog")); // true
    println!("{}", is_pangram("whatdoesthefoxsay")); // false
}