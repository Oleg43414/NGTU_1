package main

import (
	"fmt"
	"math"
)

func main() {
	// Параметры интервала и шага
	const xStart = -4.0 // Начало интервала
	const xEnd = 8.0    // Конец интервала
	const dx = 0.5      // Шаг вычислений
	x := xStart         // Текущее значение x

	// Заголовок таблицы
	fmt.Println("x\t| y")
	fmt.Println("----------------")

	// Основной цикл вычислений
	for x <= xEnd+1e-6 { // Учет погрешности для чисел с плавающей точкой
		var yStr string // Строковое представление y

		// Ветви кусочно-заданной функции
		if x < -3.0 {
			yStr = "не определено"
		} else if -3.0 <= x && x < -1.0 {
			y := x + 3
			yStr = fmt.Sprintf("%.1f", math.Round(y*10)/10)
		} else if -1.0 <= x && x < 0.0 {
			yStr = "2"
		} else if 0.0 <= x && x < 1.0 {
			y := x + 2
			yStr = fmt.Sprintf("%.1f", math.Round(y*10)/10)
		} else if 1.0 <= x && x < 3.0 {
			y := x + 2
			yStr = fmt.Sprintf("%.1f", math.Round(y*10)/10)
		} else if 3.0 <= x && x < 5.0 {
			yStr = "5"
		} else if 5.0 <= x && x <= 7.0 {
			y := x
			yStr = fmt.Sprintf("%.1f", math.Round(y*10)/10)
		} else {
			yStr = "не определено"
		}

		// Форматированный вывод строки таблицы
		fmt.Printf("%.1f\t| %s\n", x, yStr)
		x = math.Round((x + dx) * 10) / 10 // Корректное увеличение x
	}
}