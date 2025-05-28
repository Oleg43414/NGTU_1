import java.util.Scanner;

public class DigitOps {
    /**
     * Вычисляет сумму и произведение цифр числа, а также модуль их разности.
     * 
     * @param num Натуральное число для обработки
     */
    public static void calculateDigitOps(int num) {
        int sum = 0, product = 1;
        int original = num;
        
        while (num > 0) {
            int digit = num % 10;
            sum += digit;
            product *= digit;
            num /= 10;
        }
        
        System.out.println(sum + " " + product + " " + Math.abs(sum - product));
    }
    
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int N = scanner.nextInt();
        calculateDigitOps(N);
        scanner.close();
    }
}