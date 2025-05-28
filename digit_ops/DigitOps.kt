/**
 * Вычисляет сумму и произведение цифр числа, а также модуль их разности.
 * 
 * @param num Натуральное число для обработки
 */

fun calculateDigitOps(num: Int) {
    var n = num
    var sum = 0
    var product = 1
    
    while (n > 0) {
        val digit = n % 10
        sum += digit
        product *= digit
        n /= 10
    }
    
    println("$sum $product ${kotlin.math.abs(sum - product)}")
}

fun main() {
    val N = readLine()!!.toInt()
    calculateDigitOps(N)
}