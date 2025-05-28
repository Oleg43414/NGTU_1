/*
Package main реализует систему управления складом с поддержкой многофайловых проектов

Программа предоставляет:
- Интерактивное управление товарами через консольные команды
- Динамическое отслеживание заполненности ячеек и зон хранения
- Автоматическую валидацию операций с товарами
- Детализированные отчеты о состоянии склада

Параметры системы:
  • Зоны: A, B, C, D
  • Стеллажи: 8 на зону
  • Секции: 2 на стеллаж
  • Полки: 1 на секцию
  • Вместимость ячейки: 10 единиц
  • Общая емкость: 640 единиц

Командный интерфейс:
  ADD    <товар> <количество> <адрес> - Добавить товар
  REMOVE <товар> <количество> <адрес> - Удалить товар
  INFO                                - Статус склада

Особенности реализации:
  • Использование map для хранения состояния ячеек
  • Потоковая обработка ввода
  • Автоматический расчет процентной загрузки
  • Форматированный вывод с сортировкой данных

Обработка ошибок:
  • Некорректные команды
  • Несуществующие адреса ячеек
  • Превышение вместимости ячеек
  • Попытка удаления отсутствующего товара

Пример использования:
  >>> ADD Апельсины 5 A112
  >>> REMOVE Апельсины 3 A112
  >>> INFO
*/

package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

// Параметры склада для варианта 9
var zones = []rune{'A'}

const racksPerZone = 5
const sectionsPerRack = 5
const shelvesPerSection = 15
const cellCapacity = 10

var totalCapacity = len(zones) * racksPerZone * sectionsPerRack * shelvesPerSection * cellCapacity

type Cell struct {
	products map[string]int
	total    int
}

var warehouse = make(map[string]*Cell)

func initializeWarehouse() {
	for _, zone := range zones {
		for rack := 1; rack <= racksPerZone; rack++ {
			for section := 1; section <= sectionsPerRack; section++ {
				for shelf := 1; shelf <= shelvesPerSection; shelf++ {
					address := fmt.Sprintf("%c%d%d%02d", zone, rack, section, shelf)
					warehouse[address] = &Cell{
						products: make(map[string]int),
						total:    0,
					}
				}
			}
		}
	}
}

func isValidAddress(address string) bool {
	if len(address) != 5 {
		return false
	}
	if rune(address[0]) != 'A' {
		return false
	}

	rack, err := strconv.Atoi(string(address[1]))
	if err != nil || rack < 1 || rack > 5 {
		return false
	}

	section, err := strconv.Atoi(string(address[2]))
	if err != nil || section < 1 || section > 5 {
		return false
	}

	shelf, err := strconv.Atoi(address[3:])
	if err != nil || shelf < 1 || shelf > 15 {
		return false
	}

	return true
}

func processAdd(tokens []string) {
	if len(tokens) < 4 {
		fmt.Println("ERROR: Invalid ADD format")
		return
	}

	product := tokens[1]
	quantity, err := strconv.Atoi(tokens[2])
	if err != nil || quantity <= 0 {
		fmt.Println("ERROR: Invalid quantity")
		return
	}

	address := tokens[3]
	if !isValidAddress(address) {
		fmt.Println("ERROR: Invalid address")
		return
	}

	cell := warehouse[address]
	if cell.total+quantity > cellCapacity {
		fmt.Printf("ERROR: Exceeds cell capacity (max %d)\n", cellCapacity)
		return
	}

	cell.products[product] += quantity
	cell.total += quantity
}

func processRemove(tokens []string) {
	if len(tokens) < 4 {
		fmt.Println("ERROR: Invalid REMOVE format")
		return
	}

	product := tokens[1]
	quantity, err := strconv.Atoi(tokens[2])
	if err != nil || quantity <= 0 {
		fmt.Println("ERROR: Invalid quantity")
		return
	}

	address := tokens[3]
	if !isValidAddress(address) {
		fmt.Println("ERROR: Invalid address")
		return
	}

	cell := warehouse[address]
	if current, exists := cell.products[product]; !exists || current < quantity {
		fmt.Println("ERROR: Not enough products")
		return
	}

	cell.products[product] -= quantity
	cell.total -= quantity
	if cell.products[product] == 0 {
		delete(cell.products, product)
	}
}

func processInfo() {
	totalProducts := 0
	zoneProducts := make(map[rune]int)

	// Сбор статистики
	for address, cell := range warehouse {
		totalProducts += cell.total
		zoneProducts[rune(address[0])] += cell.total
	}

	// Вывод общей информации
	fmt.Printf("Warehouse load: %.1f%%\n", float64(totalProducts)/float64(totalCapacity)*100)

	for _, zone := range zones {
		zoneTotal := racksPerZone * sectionsPerRack * shelvesPerSection * cellCapacity
		load := float64(zoneProducts[zone]) / float64(zoneTotal) * 100
		fmt.Printf("Zone %c load: %.1f%%\n", zone, load)
	}

	// Вывод содержимого ячеек
	fmt.Println("\nNon-empty cells:")
	addresses := make([]string, 0, len(warehouse))
	for address := range warehouse {
		addresses = append(addresses, address)
	}
	sort.Strings(addresses)

	for _, address := range addresses {
		cell := warehouse[address]
		if cell.total > 0 {
			fmt.Printf("%s: ", address)
			products := make([]string, 0, len(cell.products))
			for product, qty := range cell.products {
				products = append(products, fmt.Sprintf("%s (%d)", product, qty))
			}
			sort.Strings(products)
			fmt.Println(strings.Join(products, ", "))
		}
	}

	// Вывод пустых ячеек
	fmt.Print("\nEmpty cells: ")
	emptyCells := make([]string, 0)
	for address, cell := range warehouse {
		if cell.total == 0 {
			emptyCells = append(emptyCells, address)
		}
	}
	sort.Strings(emptyCells)
	fmt.Println(strings.Join(emptyCells, " "))
}

func main() {
	initializeWarehouse()
	scanner := bufio.NewScanner(os.Stdin)

	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			continue
		}

		tokens := strings.Fields(line)
		if len(tokens) == 0 {
			continue
		}

		command := tokens[0]
		switch command {
		case "ADD":
			processAdd(tokens)
		case "REMOVE":
			processRemove(tokens)
		case "INFO":
			processInfo()
		default:
			fmt.Println("ERROR: Unknown command")
		}
	}
}
