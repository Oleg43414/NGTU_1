/**
 * @file warehouse_manager.cpp
 * @brief Программа управления складом с многоуровневой системой хранения
 * 
 * Система обеспечивает:
 * - Учет товаров в ячейках с ограниченной вместимостью (10 ед./ячейка)
 * - Обработку команд ADD, REMOVE и INFO
 * - Автоматическую инициализацию склада по параметрам варианта 3:
 *   • 4 зоны хранения (A, B, C, D)
 *   • 8 стеллажей в зоне
 *   • 2 вертикальные секции на стеллаж
 *   • 1 полка на секцию
 *   • Общая вместимость: 640 единиц товара
 * 
 * Формат адреса ячейки: [Зона][Стеллаж][Секция][Полка] (пример: "A512")
 * 
 * Основные функции:
 *   ADD <товар> <кол-во> <адрес>   - Добавление товара в ячейку
 *   REMOVE <товар> <кол-во> <адрес> - Удаление товара из ячейки
 *   INFO                            - Отчет о состоянии склада
 * 
 * Ограничения:
 * - Невозможность превысить вместимость ячейки
 * - Проверка наличия товара при удалении
 * - Автоматический расчет загруженности в %
 * 
 * @warning Нумерация для пользователя начинается с 1, но внутренне обрабатывается как целые числа
 * @note Использует стандартные контейнеры STL: map, vector
 * 
 * Словарь терминов:
 * quantity (amount) - количество
 */

#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdio>

using namespace std;

// Параметры склада для варианта 9
const char ZONES[] = {'A'};
const int NUM_ZONES = 1;
const int RACKS_PER_ZONE = 5;
const int SECTIONS_PER_RACK = 5;
const int SHELVES_PER_SECTION = 15;
const int CELL_CAPACITY = 10;
const int TOTAL_CAPACITY = NUM_ZONES * RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * CELL_CAPACITY;

struct Cell {
    map<string, int> products;
    int total = 0;
};

map<string, Cell> warehouse;

// Инициализация склада
void initialize_warehouse() {
    for (char zone : ZONES) {
        for (int rack = 1; rack <= RACKS_PER_ZONE; rack++) {
            for (int section = 1; section <= SECTIONS_PER_RACK; section++) {
                for (int shelf = 1; shelf <= SHELVES_PER_SECTION; shelf++) {
                    char address[6];
                    snprintf(address, sizeof(address), "%c%d%d%02d", zone, rack, section, shelf);
                    warehouse[address] = Cell();
                }
            }
        }
    }
}

// Проверка корректности адреса
bool is_valid_address(const string& address) {
    if (address.length() != 5) return false;
    if (address[0] != 'A') return false;
    
    int rack = address[1] - '0';
    if (rack < 1 || rack > 5) return false;
    
    int section = address[2] - '0';
    if (section < 1 || section > 5) return false;
    
    int shelf = stoi(address.substr(3,2));
    return (shelf >= 1 && shelf <= 15);
}

// Обработка команды ADD
void process_add(const vector<string>& tokens) {
    if (tokens.size() < 4) {
        cout << "ERROR: Invalid ADD format" << endl;
        return;
    }

    string product = tokens[1];
    int quantity;
    try {
        quantity = stoi(tokens[2]);
    } catch (...) {
        cout << "ERROR: Invalid quantity" << endl;
        return;
    }

    string address = tokens[3];
    if (!is_valid_address(address)) {
        cout << "ERROR: Invalid address" << endl;
        return;
    }

    Cell& cell = warehouse[address];
    if (cell.total + quantity > CELL_CAPACITY) {
        cout << "ERROR: Exceeds cell capacity (max " << CELL_CAPACITY << ")" << endl;
        return;
    }

    cell.products[product] += quantity;
    cell.total += quantity;
}

// Обработка команды REMOVE
void process_remove(const vector<string>& tokens) {
    if (tokens.size() < 4) {
        cout << "ERROR: Invalid REMOVE format" << endl;
        return;
    }

    string product = tokens[1];
    int quantity;
    try {
        quantity = stoi(tokens[2]);
    } catch (...) {
        cout << "ERROR: Invalid quantity" << endl;
        return;
    }

    string address = tokens[3];
    if (!is_valid_address(address)) {
        cout << "ERROR: Invalid address" << endl;
        return;
    }

    Cell& cell = warehouse[address];
    if (cell.products.find(product) == cell.products.end() || cell.products[product] < quantity) {
        cout << "ERROR: Not enough products" << endl;
        return;
    }

    cell.products[product] -= quantity;
    cell.total -= quantity;
    if (cell.products[product] == 0) {
        cell.products.erase(product);
    }
}

// Обработка команды INFO
void process_info() {
    int total_products = 0;
    map<char, int> zone_products;

    // Сбор статистики
    for (const auto& [address, cell] : warehouse) {
        total_products += cell.total;
        zone_products[address[0]] += cell.total;
    }

    // Вывод общей информации
    cout << "Warehouse load: " 
         << fixed << setprecision(1) 
         << (static_cast<double>(total_products) / TOTAL_CAPACITY * 100) 
         << "%" << endl;

    for (char zone : ZONES) {
        double load = static_cast<double>(zone_products[zone]) / (RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * CELL_CAPACITY) * 100;
        cout << "Zone " << zone << " load: " << fixed << setprecision(1) << load << "%" << endl;
    }

    // Вывод содержимого ячеек
    cout << "\nNon-empty cells:" << endl;
    for (const auto& [address, cell] : warehouse) {
        if (cell.total > 0) {
            cout << address << ": ";
            for (const auto& [product, qty] : cell.products) {
                cout << product << " (" << qty << "), ";
            }
            cout << endl;
        }
    }

    // Вывод пустых ячеек
    cout << "\nEmpty cells: ";
    for (const auto& [address, cell] : warehouse) {
        if (cell.total == 0) {
            cout << address << " ";
        }
    }
    cout << endl;
}

int main() {
    initialize_warehouse();
    string line;

    while (getline(cin, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        vector<string> tokens;
        string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) continue;

        string command = tokens[0];
        if (command == "ADD") {
            process_add(tokens);
        } else if (command == "REMOVE") {
            process_remove(tokens);
        } else if (command == "INFO") {
            process_info();
        } else {
            cout << "ERROR: Unknown command" << endl;
        }
    }

    return 0;
}
