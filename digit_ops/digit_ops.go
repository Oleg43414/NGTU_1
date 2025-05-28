package main

import (
	"fmt"
)

/**
 * Вычисляет сумму и произведение цифр числа, а также модуль их разности.
 * 
 * @param num Натуральное число для обработки
 */

func calculateDigitOps(num int) {
	sum, product := 0, 1
	original := num
	
	for num > 0 {
		digit := num % 10
		sum += digit
		product *= digit
		num /= 10
	}
	
	diff := sum - product
	if diff < 0 {
		diff = -diff
	}
	fmt.Printf("%d %d %d\n", sum, product, diff)
}

func main() {
	var N int
	fmt.Scan(&N)
	calculateDigitOps(N)
}