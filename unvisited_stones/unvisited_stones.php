<?php
/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param int $N Количество птиц.
 * @param int $stones Общее количество валунов.
 * @param array $birds Массив с дистанциями перелёта каждой птицы.
 * @return int Количество непосещённых валунов.
 */

function unvisitedStones($N, $stones, $birds) {
    $visited = array_fill(0, $stones + 1, false);
    foreach ($birds as $bird) {
        for ($pos = $bird; $pos <= $stones; $pos += $bird) {
            $visited[$pos] = true;
        }
    }
    return count(array_filter(array_slice($visited, 1), function($v) { return !$v; }));
}
?>