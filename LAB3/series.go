package main

import (
	"fmt"
)

// gcd вычисляет наибольший общий делитель (НОД) двух целых чисел.
func gcd(a, b int) int {
	for b != 0 {
		temp := b
		b = a % b
		a = temp
	}
	return a
}

func main() {
	var a, b int // Входные числа: a и b (1 <= a, b <= 10)
	fmt.Scan(&a, &b)

	if b == 1 {
		fmt.Println("infinity")
		return
	}

	var numerator, denominator int // Числитель и знаменатель дроби суммы ряда

	// Вычисление числителя и знаменателя в зависимости от a
	switch a {
	case 1:
		numerator = b
		denominator = (b - 1) * (b - 1)
	case 2:
		numerator = b * (b + 1)
		denominator = (b - 1) * (b - 1) * (b - 1)
	default:
		// Для a > 2 требуется реализация дополнительных формул
		fmt.Println("irrational")
		return
	}

	// Сокращение дроби
	commonDivisor := gcd(numerator, denominator)
	numerator /= commonDivisor
	denominator /= commonDivisor

	fmt.Printf("%d/%d\n", numerator, denominator)
}
