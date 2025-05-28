/**
 * Вычисляет сумму и произведение цифр числа, а также модуль их разности.
 * 
 * @param {number} num - Натуральное число для обработки
 */

function calculateDigitOps(num) {
    let sum = 0;
    let product = 1;
    let n = num;
    
    while (n > 0) {
        const digit = n % 10;
        sum += digit;
        product *= digit;
        n = Math.floor(n / 10);
    }
    
    console.log(`${sum} ${product} ${Math.abs(sum - product)}`);
}

const N = parseInt(prompt("Enter number:"));
calculateDigitOps(N);