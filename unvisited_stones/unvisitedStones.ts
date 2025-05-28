/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param N Количество птиц.
 * @param stones Общее количество валунов.
 * @param birds Массив с дистанциями перелёта каждой птицы.
 * @return Количество непосещённых валунов.
 */

function unvisitedStones(N: number, stones: number, birds: number[]): number {
    const visited: boolean[] = new Array(stones + 1).fill(false);
    for (const bird of birds) {
        for (let pos = bird; pos <= stones; pos += bird) {
            visited[pos] = true;
        }
    }
    return visited.slice(1).filter(v => !v).length;
}