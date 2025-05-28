# Находит количество валунов, которые не посетит ни одна птица.
# 
# @param n [Integer] Количество птиц.
# @param stones [Integer] Общее количество валунов.
# @param birds [Array<Integer>] Массив с дистанциями перелёта каждой птицы.
# @return [Integer] Количество непосещённых валунов.

def unvisited_stones(n, stones, birds)
    visited = Array.new(stones + 1, false)
    birds.each do |bird|
      (bird..stones).step(bird) { |pos| visited[pos] = true }
    end
    visited[1..-1].count(false)
  end