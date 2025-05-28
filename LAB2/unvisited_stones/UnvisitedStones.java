public class UnvisitedStones {
    /**
     * Находит количество валунов, которые не посетит ни одна птица.
     * 
     * @param N Количество птиц.
     * @param stones Общее количество валунов.
     * @param birds Массив с дистанциями перелёта каждой птицы.
     * @return Количество непосещённых валунов.
     */
    
    public int unvisitedStones(int N, int stones, int[] birds) {
        boolean[] visited = new boolean[stones + 1];
        for (int bird : birds) {
            for (int pos = bird; pos <= stones; pos += bird) {
                visited[pos] = true;
            }
        }
        int count = 0;
        for (int i = 1; i <= stones; i++) {
            if (!visited[i]) {
                count++;
            }
        }
        return count;
    }
}