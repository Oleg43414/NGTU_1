<?php
/**
 * Вычисляет сумму и произведение цифр числа, а также модуль их разности.
 * 
 * @param int $num Натуральное число для обработки
 */

function calculateDigitOps($num) {
    $sum = 0;
    $product = 1;
    
    while ($num > 0) {
        $digit = $num % 10;
        $sum += $digit;
        $product *= $digit;
        $num = (int)($num / 10);
    }
    
    echo $sum . " " . $product . " " . abs($sum - $product) . "\n";
}

$N = (int)trim(fgets(STDIN));
calculateDigitOps($N);
?>