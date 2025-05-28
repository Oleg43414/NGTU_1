
/**
 * Пример использования:
 * @code
 * vector<string> candidates = {"A", "B", "C"};
 * vector<vector<string>> voters = {{"A", "B", "C"}, {"B", "C", "A"}};
 * Election election(candidates, voters);
 * cout << "Борда: " << election.BordaCount() << endl;
 * @endcode
 */

 /**
 * @file ElectionSystem.cpp
 * @brief Реализация методов Борда и Кондорсе для определения победителей выборов.
 */

/**
 * @file ElectionSystem.cpp
 * @brief Реализация методов Борда и Кондорсе для определения победителей выборов.
 */

/**
 * @file ElectionSystem.cpp
 * @brief Система подсчета голосов с методами Борда и Кондорсе
 * 
 * Реализует два классических метода определения победителей выборов:
 * 1. Метод Борда - ранжирование кандидатов по набранным баллам
 * 2. Метод Кондорсе - поиск кандидата, побеждающего всех в попарных сравнениях
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <utility> // Для std::move

using namespace std;

class Election {
private:
    // Список кандидатов (уникальные идентификаторы)
    vector<string> candidates;
    
    // Матрица предпочтений избирателей:
    // Каждый внутренний вектор - ранжированный список кандидатов от наиболее предпочтительного
    vector<vector<string>> votersPreferences;
    
    // Кэш позиций кандидатов для быстрого доступа:
    // Для каждого избирателя хранится словарь [кандидат → позиция в рейтинге]
    vector<unordered_map<string, int>> voterRankMaps;

    /**
     * @brief Инициализирует кэш позиций кандидатов
     * 
     * Создает для каждого избирателя словарь, позволяющий быстро определить
     * позицию кандидата в его предпочтениях (0 - самый предпочтительный)
     */
    void initRankCache() {
        voterRankMaps.clear();
        for (const auto& voter : votersPreferences) {
            unordered_map<string, int> rankMap;
            for (size_t i = 0; i < voter.size(); ++i) {
                rankMap[voter[i]] = static_cast<int>(i); // Кандидат → его позиция
            }
            voterRankMaps.push_back(move(rankMap)); // Оптимизация через перемещение
        }
    }

    /**
     * @brief Проверяет корректность входных данных
     * @throw invalid_argument если данные некорректны
     * 
     * Проверяет:
     * 1. Не пуст ли список кандидатов или избирателей
     * 2. Соответствует ли количество кандидатов в предпочтениях
     * 3. Содержатся ли все кандидаты в каждом голосе
     */
    void validateInput() const {
        if (candidates.empty() || votersPreferences.empty()) {
            throw invalid_argument("Список кандидатов или избирателей пуст");
        }
        
        for (const auto& pref : votersPreferences) {
            if (pref.size() != candidates.size()) {
                throw invalid_argument("Не все избиратели указали предпочтения для всех кандидатов");
            }
            
            // Проверка наличия всех кандидатов в предпочтениях
            for (const auto& cand : candidates) {
                if (find(pref.begin(), pref.end(), cand) == pref.end()) {
                    throw invalid_argument("Предпочтения избирателя не содержат всех кандидатов");
                }
            }
        }
    }

public:
    /**
     * @brief Конструктор системы голосования
     * @param cand Список кандидатов
     * @param prefs Матрица предпочтений избирателей
     * 
     * Принимает данные, проверяет их корректность и инициализирует кэш
     */
    Election(vector<string> cand, vector<vector<string>> prefs) 
        : candidates(move(cand)), votersPreferences(move(prefs)) {
        validateInput();
        initRankCache();
    }

    /**
     * @brief Подсчет результатов по методу Борда
     * @return Имя победителя или "Ничья"/"Нет данных"
     * 
     * Каждый кандидат получает баллы:
     * - За 1 место: N-1 баллов
     * - За 2 место: N-2 баллов
     * - ...
     * - За последнее место: 0 баллов
     * 
     * Побеждает кандидат с максимальной суммой баллов
     */
    string BordaCount() const {
        unordered_map<string, int> scores; // Словарь [кандидат → сумма баллов]
        const int n = static_cast<int>(candidates.size());

        // Подсчет баллов для каждого кандидата
        for (const auto& voter : votersPreferences) {
            for (size_t i = 0; i < voter.size(); ++i) {
                scores[voter[i]] += (n - 1 - static_cast<int>(i));
            }
        }

        if (scores.empty()) return "Нет данных";

        // Поиск максимального значения
        auto maxIt = max_element(scores.begin(), scores.end(), 
            [](const pair<string, int>& a, const pair<string, int>& b) { 
                return a.second < b.second; 
            });

        // Проверка на ничью (несколько кандидатов с максимальным баллом)
        int count = count_if(scores.begin(), scores.end(), 
            [&maxIt](const pair<string, int>& pair) { 
                return pair.second == maxIt->second; 
            });

        return (count == 1) ? maxIt->first : "Ничья";
    }

    /**
     * @brief Поиск победителя по методу Кондорсе
     * @return Имя победителя или "Нет победителя"
     * 
     * Победитель Кондорсе - кандидат, который побеждает всех остальных
     * в попарных сравнениях (большинство избирателей предпочитают его другому кандидату)
     */
    string CondorcetWinner() const {
        // Проверяем каждого кандидата
        for (const auto& candidateA : candidates) {
            bool dominatesAll = true; // Флаг "побеждает всех"
            
            // Сравниваем с каждым другим кандидатом
            for (const auto& candidateB : candidates) {
                if (candidateA == candidateB) continue; // Не сравниваем с собой

                int aWins = 0; // Счетчик побед A над B
                
                // Подсчет предпочтений всех избирателей
                for (const auto& rankMap : voterRankMaps) {
                    try {
                        // Если A выше в рейтинге, чем B
                        if (rankMap.at(candidateA) < rankMap.at(candidateB)) {
                            aWins++;
                        }
                    } catch (const out_of_range& e) {
                        throw runtime_error("Ошибка в данных: отсутствует кандидат в предпочтениях избирателя");
                    }
                }

                // Если A не получил большинство против B
                if (aWins <= static_cast<int>(votersPreferences.size()) / 2) {
                    dominatesAll = false;
                    break;
                }
            }
            
            // Если кандидат побеждает всех остальных
            if (dominatesAll) return candidateA;
        }
        return "Нет победителя";
    }
};

/**
 * @brief Пример использования системы голосования
 */
int main() {
    try {
        // Тестовые данные
        vector<string> candidates = {"A", "B", "C"};
        vector<vector<string>> preferences = {
            {"B", "A", "C"}, // 1-й избиратель: A > B > C
            {"B", "C", "A"},
            {"A", "B", "C"},
            {"C", "B", "A"} // 2-й избиратель: B > C > A
        };
        
        // Создание системы голосования
        Election election(move(candidates), move(preferences));
        
        // Вывод результатов
        cout << "Победитель по Борда: " << election.BordaCount() << endl;
        cout << "Победитель по Кондорсе: " << election.CondorcetWinner() << endl;
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}