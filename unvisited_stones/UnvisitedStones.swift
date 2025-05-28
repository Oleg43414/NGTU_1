/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param N Количество птиц.
 * @param stones Общее количество валунов.
 * @param birds Массив с дистанциями перелёта каждой птицы.
 * @return Количество непосещённых валунов.
 */
 
func unvisitedStones(_ N: Int, _ stones: Int, _ birds: [Int]) -> Int {
    var visited = [Bool](repeating: false, count: stones + 1)
    for bird in birds {
        var pos = bird
        while pos <= stones {
            visited[pos] = true
            pos += bird
        }
    }
    return visited.dropFirst().filter { !$0 }.count
}