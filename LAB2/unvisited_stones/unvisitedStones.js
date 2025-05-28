/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param {number} N Количество птиц.
 * @param {number} stones Общее количество валунов.
 * @param {number[]} birds Массив с дистанциями перелёта каждой птицы.
 * @return {number} Количество непосещённых валунов.
 */

function unvisitedStones(N, stones, birds) {
    const visited = new Array(stones + 1).fill(false);
    for (const bird of birds) {
        for (let pos = bird; pos <= stones; pos += bird) {
            visited[pos] = true;
        }
    }
    return visited.slice(1).filter(v => !v).length;
}