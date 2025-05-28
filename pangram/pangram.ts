/**
 * Проверяет, является ли строка панграммой
 * @param sentence Входная строка
 * @returns true если строка содержит все буквы алфавита, иначе false
 */

function isPangram(sentence: string): boolean {
    const alphabet = 'abcdefghijklmnopqrstuvwxyz';
    const lowerSentence = sentence.toLowerCase();
    return alphabet.split('').every(letter => lowerSentence.includes(letter));
}

console.log(isPangram("thequickbrownfoxjumpsoverthelazydog")); // true
console.log(isPangram("whatdoesthefoxsay")); // false