/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param N Количество птиц.
 * @param stones Общее количество валунов.
 * @param birds Массив с дистанциями перелёта каждой птицы.
 * @return Количество непосещённых валунов.
 */
 
fun unvisitedStones(N: Int, stones: Int, birds: IntArray): Int {
    val visited = BooleanArray(stones + 1) // Индексация с 1
    for (bird in birds) {
        var pos = bird
        while (pos <= stones) {
            visited[pos] = true
            pos += bird
        }
    }
    return visited.count { !it } - 1 // Игнорируем 0-й индекс
}