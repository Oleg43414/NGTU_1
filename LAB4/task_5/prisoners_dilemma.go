/*
Package prisoners_dilemma реализует игру "Дилемма заключенного".
Содержит три стратегии: всегда сотрудничать, всегда предавать и "око за око".
*/
package main

import (
	"fmt"
	"math/rand"
	"time"
)

type Strategy func(int, []bool, []bool) bool

type PrisonersDilemma struct {
	rounds    int
	strategyA Strategy
	strategyB Strategy
	choicesA  []bool
	choicesB  []bool
	scoreA    int
	scoreB    int
}

/**
 * @brief Создает новый экземпляр игры.
 * @param a Стратегия игрока A.
 * @param b Стратегия игрока B.
 */
func NewPrisonersDilemma(a, b Strategy) *PrisonersDilemma {
	rand.Seed(time.Now().UnixNano())
	return &PrisonersDilemma{
		rounds:    rand.Intn(101) + 100,
		strategyA: a,
		strategyB: b,
	}
}

/**
 * @brief Рассчитывает очки на основе выборов игроков.
 */
func (pd *PrisonersDilemma) calculateScores(a, b bool) {
	if !a && !b {
		pd.scoreA += 4
		pd.scoreB += 4
	} else if a && !b {
		pd.scoreB += 20
	} else if !a && b {
		pd.scoreA += 20
	} else {
		pd.scoreA += 24
		pd.scoreB += 24
	}
}

/**
 * @brief Запускает серию раундов игры.
 */
func (pd *PrisonersDilemma) Run() {
	for round := 0; round < pd.rounds; round++ {
		choiceA := pd.strategyA(round, pd.choicesA, pd.choicesB)
		choiceB := pd.strategyB(round, pd.choicesB, pd.choicesA)

		pd.choicesA = append(pd.choicesA, choiceA)
		pd.choicesB = append(pd.choicesB, choiceB)

		pd.calculateScores(choiceA, choiceB)
	}
}

// Примеры стратегий
func AlwaysCooperate(round int, self, enemy []bool) bool {
	return true
}

func AlwaysBetray(round int, self, enemy []bool) bool {
	return false
}

func TitForTat(round int, self, enemy []bool) bool {
	if round == 0 {
		return true
	}
	if len(enemy) == 0 {
		return true
	}
	return enemy[len(enemy)-1]
}

func main() {
	game1 := NewPrisonersDilemma(TitForTat, AlwaysCooperate)
	game1.Run()
	fmt.Printf("TitForTat vs Всегда сотрудничать: %d - %d\n", game1.scoreA, game1.scoreB)

	game2 := NewPrisonersDilemma(TitForTat, AlwaysBetray)
	game2.Run()
	fmt.Printf("TitForTat vs Всегда предавать: %d - %d\n", game2.scoreA, game2.scoreB)
}
