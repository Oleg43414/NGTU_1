use std::io;

/// Вычисляет сумму и произведение цифр числа, а также модуль их разности.
/// 
/// # Аргументы
/// * `num` - Натуральное число для обработки

fn calculate_digit_ops(num: i32) {
    let mut n = num;
    let mut sum = 0;
    let mut product = 1;
    
    while n > 0 {
        let digit = n % 10;
        sum += digit;
        product *= digit;
        n /= 10;
    }
    
    println!("{} {} {}", sum, product, (sum - product).abs());
}

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    let N: i32 = input.trim().parse().unwrap();
    calculate_digit_ops(N);
}