/*
Package election реализует методы подсчета голосов Борда и Кондорсе.

Основные возможности:
- Определение победителя по методу Борда с учетом позиций.
- Поиск победителя Кондорсе через попарные сравнения.
- Обработка крайних случаев: ничьи, отсутствие победителя.

Пример использования:

	candidates := []string{"A", "B", "C"}
	voters := [][]string{{"A", "B", "C"}, {"B", "C", "A"}}
	e := NewElection(candidates, voters)
	fmt.Println("Борда:", e.BordaCount())
*/
package election

import (
	"errors"
)

// Election содержит данные о кандидатах и предпочтениях избирателей.
type Election struct {
	Candidates        []string
	VotersPreferences [][]string
	voterRankMaps     []map[string]int // Кэш позиций для быстрого доступа
}

// NewElection создает объект Election с проверкой входных данных.
// Возвращает ошибку, если списки пусты.
func NewElection(candidates []string, prefs [][]string) (*Election, error) {
	if len(candidates) == 0 || len(prefs) == 0 {
		return nil, errors.New("некорректные входные данные")
	}

	e := &Election{
		Candidates:        candidates,
		VotersPreferences: prefs,
	}
	e.initRankCache()
	return e, nil
}

// initRankCache инициализирует кэш позиций кандидатов.
func (e *Election) initRankCache() {
	e.voterRankMaps = make([]map[string]int, len(e.VotersPreferences))
	for i, voter := range e.VotersPreferences {
		rankMap := make(map[string]int)
		for pos, candidate := range voter {
			rankMap[candidate] = pos
		}
		e.voterRankMaps[i] = rankMap
	}
}

// BordaCount вычисляет победителя по методу Борда.
// Возвращает имя победителя или "Ничья".
func (e *Election) BordaCount() string {
	scores := make(map[string]int)
	n := len(e.Candidates)

	for _, voter := range e.VotersPreferences {
		for pos, candidate := range voter {
			scores[candidate] += (n - 1 - pos)
		}
	}

	maxScore := -1
	winner := ""
	isTie := false
	for candidate, score := range scores {
		if score > maxScore {
			maxScore = score
			winner = candidate
			isTie = false
		} else if score == maxScore {
			isTie = true
		}
	}

	if isTie {
		return "Ничья"
	}
	return winner
}

// CondorcetWinner ищет победителя по методу Кондорсе.
// Возвращает имя победителя или "Нет победителя".
func (e *Election) CondorcetWinner() string {
	for _, candidateA := range e.Candidates {
		isDominant := true
		for _, candidateB := range e.Candidates {
			if candidateA == candidateB {
				continue
			}

			aWins := 0
			for _, rankMap := range e.voterRankMaps {
				if rankMap[candidateA] < rankMap[candidateB] {
					aWins++
				}
			}

			if aWins <= len(e.VotersPreferences)/2 {
				isDominant = false
				break
			}
		}
		if isDominant {
			return candidateA
		}
	}
	return "Нет победителя"
}
