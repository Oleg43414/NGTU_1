package main

import (
	"fmt"
	"math"
)

// Функция уравнения: 2ln(x) - 0.5x + 1
func f(x float64) float64 {
	return 2*math.Log(x) - 0.5*x + 1
}

// Производная функции: 2/x - 0.5
func df(x float64) float64 {
	return 2/x - 0.5
}

// Метод половинного деления
func bisection(a, b, eps float64) {
	n := 0
	fmt.Println("Метод половинного деления:")
	fmt.Println("N\t a\t\t b\t\t b - a")
	for math.Abs(b-a) > eps {
		c := (a + b) / 2
		fmt.Printf("%d\t %.6f\t %.6f\t %.6f\n", n, a, b, b-a)
		if f(c)*f(a) < 0 {
			b = c
		} else {
			a = c
		}
		n++
	}
	fmt.Printf("Корень: %.6f\n", (a+b)/2)
}

// Метод Ньютона
func newton(x0, eps float64) {
	n := 0
	x := x0
	fmt.Println("Метод Ньютона:")
	fmt.Println("N\t Xn\t\t Xn+1\t\t Xn+1 - Xn")
	for {
		fx := f(x)
		dx := df(x)
		xNext := x - fx/dx
		fmt.Printf("%d\t %.6f\t %.6f\t %.6f\n", n, x, xNext, math.Abs(xNext-x))
		if math.Abs(xNext-x) < eps {
			break
		}
		x = xNext
		n++
	}
	fmt.Printf("Корень: %.6f\n", x)
}

// Метод простых итераций
func iteration(x0, eps float64) {
	n := 0
	x := x0
	fmt.Println("Метод простых итераций:")
	fmt.Println("N\t Xn\t\t Xn+1\t\t Xn+1 - Xn")
	for {
		xNext := (2*math.Log(x) + 1) / 0.5
		fmt.Printf("%d\t %.6f\t %.6f\t %.6f\n", n, x, xNext, math.Abs(xNext-x))
		if math.Abs(xNext-x) < eps {
			break
		}
		x = xNext
		n++
	}
	fmt.Printf("Корень: %.6f\n", x)
}

func main() {
	eps := 1e-4
	a, b := 0.5, 1.0 // Интервал для метода половинного деления
	x0Newton := 0.75 // Начальное приближение для Ньютона
	x0Iter := 0.75   // Начальное приближение для итераций

	bisection(a, b, eps)
	newton(x0Newton, eps)
	iteration(x0Iter, eps)
}

/**
Метод Ньютона: Квадратичная сходимость (наибыстрейший).
Метод простых итераций: Линейная сходимость.
Метод половинного деления: Линейная сходимость (самый медленный).
*/

/*
go run nonlinear_equation_solver.go
*/