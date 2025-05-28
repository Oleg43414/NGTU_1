package main

/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param N Количество птиц.
 * @param stones Общее количество валунов.
 * @param birds Массив с дистанциями перелёта каждой птицы.
 * @return Количество непосещённых валунов.
 */
 
func unvisitedStones(N int, stones int, birds []int) int {
    visited := make([]bool, stones+1)
    for _, bird := range birds {
        for pos := bird; pos <= stones; pos += bird {
            visited[pos] = true
        }
    }
    count := 0
    for _, v := range visited[1:] {
        if !v {
            count++
        }
    }
    return count
}