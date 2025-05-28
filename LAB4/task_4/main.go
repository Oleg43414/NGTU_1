/*
Пример использования (Go)
Борда: Вася
Кондорсе: Нет победителя
*/

func main() {
	candidates := []string{"Петя", "Вася", "Оля"}
	voters := [][]string{
		{"Вася", "Оля", "Петя"},
		{"Оля", "Петя", "Вася"},
		{"Вася", "Петя", "Оля"},
	}

	election, err := NewElection(candidates, voters)
	if err != nil {
		fmt.Println("Ошибка:", err)
		return
	}

	fmt.Println("Борда:", election.BordaCount())
	fmt.Println("Кондорсе:", election.CondorcetWinner())
}
