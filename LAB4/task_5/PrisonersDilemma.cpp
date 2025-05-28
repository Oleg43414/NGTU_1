/**
 * @file PrisonersDilemma.cpp
 * @brief Реализация игры "Дилемма заключенного" с тремя стратегиями.
 */

#include <iostream>
#include <vector>
#include <functional>
#include <random>

using namespace std;

using Strategy = function<bool(int, const vector<bool>&, const vector<bool>&)>;

class PrisonersDilemma {
private:
    int rounds;
    Strategy strategyA;
    Strategy strategyB;
    vector<bool> choicesA;
    vector<bool> choicesB;
    int scoreA = 0;
    int scoreB = 0;

    /**
     * @brief Генерирует случайное количество раундов от 100 до 200.
     */
    int generateRounds() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(100, 200);
        return distr(gen);
    }

    /**
     * @brief Рассчитывает очки для игроков на основе их выборов.
     * @param a Выбор игрока A.
     * @param b Выбор игрока B.
     */
    void calculateScores(bool a, bool b) {
        if (!a && !b) { // Оба предали
            scoreA += 4;
            scoreB += 4;
        } else if (a && !b) { // A сотрудничал, B предал
            scoreB += 20;
        } else if (!a && b) { // A предал, B сотрудничал
            scoreA += 20;
        } else { // Оба сотрудничали
            scoreA += 24;
            scoreB += 24;
        }
    }

public:
    /**
     * @brief Конструктор класса.
     * @param a Стратегия игрока A.
     * @param b Стратегия игрока B.
     */
    PrisonersDilemma(Strategy a, Strategy b) 
        : strategyA(a), strategyB(b), rounds(generateRounds()) {}

    /**
     * @brief Запускает игру на заданное количество раундов.
     */
    void run() {
        for (int round = 0; round < rounds; ++round) {
            bool choiceA = strategyA(round, choicesA, choicesB);
            bool choiceB = strategyB(round, choicesB, choicesA);
            
            choicesA.push_back(choiceA);
            choicesB.push_back(choiceB);
            
            calculateScores(choiceA, choiceB);
        }
    }

    int getScoreA() const { return scoreA; }
    int getScoreB() const { return scoreB; }
};

// Примеры стратегий
bool alwaysCooperate(int, const vector<bool>&, const vector<bool>&) {
    return true;
}

bool alwaysBetray(int, const vector<bool>&, const vector<bool>&) {
    return false;
}

bool titForTat(int round, const vector<bool>& self, const vector<bool>& enemy) {
    if (round == 0) return true;
    return enemy.back();
}

int main() {
    PrisonersDilemma game1(titForTat, alwaysCooperate);
    game1.run();
    cout << "TitForTat vs Всегда сотрудничать: " 
         << game1.getScoreA() << " - " << game1.getScoreB() << endl;

    PrisonersDilemma game2(titForTat, alwaysBetray);
    game2.run();
    cout << "TitForTat vs Всегда предавать: " 
         << game2.getScoreA() << " - " << game2.getScoreB() << endl;

    
    PrisonersDilemma game3(titForTat, alwaysBetray);
    game3.run();
    cout << "TitForTat vs Око за око: " 
         << game3.getScoreA() << " - " << game3.getScoreB() << endl; 0;
    return 0;
}
