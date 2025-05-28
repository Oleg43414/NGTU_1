/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param n Количество птиц.
 * @param stones Общее количество валунов.
 * @param birds Массив с дистанциями перелёта каждой птицы.
 * @return Количество непосещённых валунов.
 */
 
 fn unvisited_stones(n: usize, stones: usize, birds: &[usize]) -> usize {
    let mut visited = vec![false; stones + 1];
    for &bird in birds {
        let mut pos = bird;
        while pos <= stones {
            visited[pos] = true;
            pos += bird;
        }
    }
    visited.iter().skip(1).filter(|&&v| !v).count()
}