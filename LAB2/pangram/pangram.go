package main

import (
	"fmt"
	"strings"
)

// IsPangram проверяет, является ли строка панграммой
func IsPangram(sentence string) bool {
	lowerSentence := strings.ToLower(sentence)
	for c := 'a'; c <= 'z'; c++ {
		if !strings.ContainsRune(lowerSentence, c) {
			return false
		}
	}
	return true
}

func main() {
	fmt.Println(IsPangram("thequickbrownfoxjumpsoverthelazydog")) // true
	fmt.Println(IsPangram("whatdoesthefoxsay")) // false
}