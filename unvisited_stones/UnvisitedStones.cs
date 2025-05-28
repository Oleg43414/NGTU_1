using System;
using System.Linq;

public class Solution {
    /**
     * Находит количество валунов, которые не посетит ни одна птица.
     * 
     * @param N Количество птиц.
     * @param stones Общее количество валунов.
     * @param birds Массив с дистанциями перелёта каждой птицы.
     * @return Количество непосещённых валунов.
     */
     
    public int UnvisitedStones(int N, int stones, int[] birds) {
        bool[] visited = new bool[stones + 1];
        foreach (int bird in birds) {
            for (int pos = bird; pos <= stones; pos += bird) {
                visited[pos] = true;
            }
        }
        return visited.Skip(1).Count(x => !x);
    }
}