#include <iostream>
#include <unordered_set>
#include <algorithm>

using namespace std;

/**
 * Проверяет, является ли строка панграммой
 * @param sentence Входная строка
 * @return true если строка содержит все буквы алфавита, иначе false
 */

bool isPangram(const string& sentence) {
    unordered_set<char> letters;
    for (char c : sentence) {
        if (isalpha(c)) {
            letters.insert(tolower(c));
        }
    }
    return letters.size() == 26;
}

int main() {
    cout << boolalpha << isPangram("thequickbrownfoxjumpsoverthelazydog") << endl; // true
    cout << boolalpha << isPangram("whatdoesthefoxsay") << endl; // false
    return 0;
}