<?php

/**
 * Проверяет, является ли строка панграммой
 * @param string $sentence Входная строка
 * @return bool true если строка содержит все буквы алфавита, иначе false
 */

function isPangram($sentence) {
    $alphabet = range('a', 'z');
    $lowerSentence = strtolower($sentence);
    foreach ($alphabet as $letter) {
        if (strpos($lowerSentence, $letter) === false) {
            return false;
        }
    }
    return true;
}

echo isPangram("thequickbrownfoxjumpsoverthelazydog") ? 'true' : 'false'; // true
echo "\n";
echo isPangram("whatdoesthefoxsay") ? 'true' : 'false'; // false
?>