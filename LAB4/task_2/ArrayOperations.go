/*
Package main реализует задание по работе с массивами:
1. Генерация массива из n целых чисел (n >= 10) в диапазоне [-100, 100].
2. Вычисление средних арифметических положительных и отрицательных элементов.
   Обмен местами максимального и минимального элементов.
3. Поиск самой длинной убывающей последовательности.
4. Вывод массива N×5, начиная с 1000.
*/

package main

import (
    "fmt"
    "math/rand"
    "time"
)

// GenerateArray генерирует массив случайных целых чисел в диапазоне [-100, 100].
// n: размер массива (n >= 10).
func GenerateArray(n int) []int {
    rand.Seed(time.Now().UnixNano())
    arr := make([]int, n)
    for i := range arr {
        arr[i] = rand.Intn(201) - 100
    }
    return arr
}

// ProcessArray вычисляет средние значения и меняет местами max/min.
// Возвращает средние и изменённый массив.
func ProcessArray(arr []int) (float64, float64, []int) {
    sumPos, countPos := 0, 0
    sumNeg, countNeg := 0, 0

    for _, num := range arr {
        if num > 0 {
            sumPos += num
            countPos++
        } else if num < 0 {
            sumNeg += num
            countNeg++
        }
    }

    avgPos := 0.0
    if countPos > 0 {
        avgPos = float64(sumPos) / float64(countPos)
    }

    avgNeg := 0.0
    if countNeg > 0 {
        avgNeg = float64(sumNeg) / float64(countNeg)
    }

    // Поиск индексов min и max
    minIdx, maxIdx := 0, 0
    for i, num := range arr {
        if num < arr[minIdx] {
            minIdx = i
        }
        if num > arr[maxIdx] {
            maxIdx = i
        }
    }
    arr[minIdx], arr[maxIdx] = arr[maxIdx], arr[minIdx]

    return avgPos, avgNeg, arr
}

// FindDecreasingSequence находит самую длинную убывающую последовательность.
func FindDecreasingSequence(arr []int) []int {
    current := []int{arr[0]}
    longest := []int{}

    for _, num := range arr[1:] {
        if num < current[len(current)-1] {
            current = append(current, num)
        } else {
            if len(current) > len(longest) {
                longest = current
            }
            current = []int{num}
        }
    }

    if len(current) > len(longest) {
        longest = current
    }

    return longest
}

// PrintFormattedArray выводит массив в формате N×5.
func PrintFormattedArray(N int) {
    start := 1000
    for i := 0; i < N; i++ {
        for j := 0; j < 5; j++ {
            fmt.Printf("%d ", start + i*5 + j)
        }
        fmt.Println()
    }
}

func main() {
    // Пункт 1
    arr := GenerateArray(10)
    fmt.Println("1. Исходный массив:", arr)

    // Пункт 2
    avgPos, avgNeg, processedArr := ProcessArray(arr)
    fmt.Printf("\n2. Среднее положительных: %.2f\n", avgPos)
    fmt.Printf("Среднее отрицательных: %.2f\n", avgNeg)
    fmt.Println("Массив после замены:", processedArr)

    // Пункт 3
    seq := FindDecreasingSequence(processedArr)
    fmt.Println("\n3. Убывающая последовательность:", seq)

    // Пункт 4
    fmt.Println("\n4. Форматированный массив (N=4):")
    PrintFormattedArray(4)
}
