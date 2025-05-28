def unvisited_stones(N: int, stones: int, birds: list[int]) -> int:
    """
    Находит количество валунов, которые не посетит ни одна птица.

    :param N: Количество птиц.
    :param stones: Общее количество валунов.
    :param birds: Список с дистанциями перелёта каждой птицы.
    :return: Количество непосещённых валунов.
    """
    if stones <= 0:
        return 0  # Если валунов нет, возвращаем 0
    
    visited = [False] * (stones + 1)  # visited[0] не используется, камни от 1 до stones
    
    for bird in birds:
        if bird <= 0:
            continue  # Пропускаем некорректные дистанции
        for pos in range(bird, stones + 1, bird):
            visited[pos] = True
    
    # Считаем непосещённые валуны (индексы от 1 до stones)
    return sum(1 for pos in range(1, stones + 1) if not visited[pos])



print(unvisited_stones(2, 10, [2, 5]))  # Должно вернуть 4 (1, 3, 7, 9)
print(unvisited_stones(3, 15, [3, 4, 6]))  # Должно вернуть 8 (1, 2, 5, 7, 10, 11, 13, 14)
print(unvisited_stones(1, 100, [1]))  # Должно вернуть 0 (все камни посещены)
