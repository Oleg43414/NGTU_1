class ElectionSystem:
    """
    Класс для определения победителей выборов по методам Борда и Кондорсе.

    Атрибуты:
        candidates (list): Список кандидатов.
        voters_preferences (list): Список предпочтений избирателей.
    """

    def __init__(self, candidates, voters_preferences):
        """
        Инициализация системы выборов.

        Args:
            candidates (list): Список кандидатов.
            voters_preferences (list): Список предпочтений избирателей. 
                Каждый элемент списка — это список кандидатов в порядке убывания предпочтений.
        """
        self.candidates = candidates
        self.voters_preferences = voters_preferences

    def borda_count(self):
        """
        Вычисляет победителя по методу Борда.

        Returns:
            str: Имя кандидата с наибольшим количеством баллов.
        """
        scores = {candidate: 0 for candidate in self.candidates}
        n = len(self.candidates)
        
        for voter in self.voters_preferences:
            for idx, candidate in enumerate(voter):
                scores[candidate] += (n - 1 - idx)
        
        max_score = max(scores.values())
        winners = [k for k, v in scores.items() if v == max_score]
        return winners[0] if len(winners) == 1 else "Ничья"

    def condorcet_winner(self):
        """
        Вычисляет победителя по методу Кондорсе.

        Returns:
            str: Имя кандидата, побеждающего всех в парных сравнениях, или None, если такого нет.
        """
        for candidate_a in self.candidates:
            is_winner = True
            for candidate_b in self.candidates:
                if candidate_a == candidate_b:
                    continue
                a_vs_b = 0
                for voter in self.voters_preferences:
                    if voter.index(candidate_a) < voter.index(candidate_b):
                        a_vs_b += 1
                if a_vs_b <= len(self.voters_preferences) / 2:
                    is_winner = False
                    break
            if is_winner:
                return candidate_a
        return None

    def get_winners(self):
        """
        Возвращает победителей по обоим методам.

        Returns:
            tuple: (победитель Борда, победитель Кондорсе, комментарий)
        """
        borda = self.borda_count()
        condorcet = self.condorcet_winner()
        comment = ""
        if borda != condorcet and condorcet is not None:
            comment = "Разные методы дали разных победителей!"
        return (borda, condorcet, comment)

# Пример использования
if __name__ == "__main__":
    candidates = ["Петя", "Вася", "Оля"]
    voters = [
        ["Вася", "Оля", "Петя"],
        ["Оля", "Петя", "Вася"],
        ["Вася", "Петя", "Оля"]
    ]

    election = ElectionSystem(candidates, voters)
    borda_winner, condorcet_winner, comment = election.get_winners()

    print(f"Победитель по методу Борда: {borda_winner}")
    print(f"Победитель по методу Кондорсе: {condorcet_winner if condorcet_winner else 'Нет победителя'}")
    print(comment)
