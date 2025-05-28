# Вычисляет сумму и произведение цифр числа, а также модуль их разности.
#
# @param num [Integer] Натуральное число для обработки

def calculate_digit_ops(num)
    sum = 0
    product = 1
    n = num
    
    while n > 0
      digit = n % 10
      sum += digit
      product *= digit
      n /= 10
    end
    
    puts "#{sum} #{product} #{(sum - product).abs}"
  end
  
  N = gets.chomp.to_i
  calculate_digit_ops(N)