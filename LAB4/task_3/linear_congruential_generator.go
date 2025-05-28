package main

import (
	"fmt"
)

/**
 * @brief Генерирует последовательность чисел с помощью линейного конгруэнтного генератора и находит индекс начала цикла.
 * @param X0 Начальное значение (seed).
 * @param A Множитель.
 * @param B Инкремент.
 * @param C Модуль.
 */
func generateLCG(X0, A, B, C int) {
	seen := make(map[int]int) // Хранит число и его индекс
	sequence := []int{}       // Последовательность чисел
	current := X0
	index := 0

	// Генерация чисел до обнаружения повтора
	for {
		if idx, exists := seen[current]; exists {
			// Вывод последовательности
			for _, num := range sequence {
				fmt.Printf("%d ", num)
			}
			// Вывод индекса начала цикла
			fmt.Printf("\n%d\n", idx)
			break
		}
		seen[current] = index
		sequence = append(sequence, current)
		current = (A*current + B) % C
		index++
	}
}

func main() {
	var X0, A, B, C int
	// Ввод параметров
	fmt.Scan(&X0, &A, &B, &C)
	generateLCG(X0, A, B, C)
}
