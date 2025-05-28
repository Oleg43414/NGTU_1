/// Вычисляет сумму и произведение цифр числа, а также модуль их разности.
/// 
/// - Parameter num: Натуральное число для обработки

func calculateDigitOps(num: Int) {
    var n = num
    var sum = 0
    var product = 1
    
    while n > 0 {
        let digit = n % 10
        sum += digit
        product *= digit
        n /= 10
    }
    
    print("\(sum) \(product) \(abs(sum - product))")
}

let N = Int(readLine()!)!
calculateDigitOps(num: N)