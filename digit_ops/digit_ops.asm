; -------------------------------------------------------------------------- ;
; NASM x86 assembly                                                          ;
; Вычисляет сумму и произведение цифр числа, а также модуль их разности.     ;
;                                                                            ;
; Вход:                                                                      ;
;   - Вводится натуральное число с клавиатуры                                ;
;                                                                            ;
; Выход:                                                                     ;
;   - Три числа через пробел: сумма цифр, произведение цифр, модуль разности ;
; -------------------------------------------------------------------------- ;

section .data
    output_format db "%d %d %d", 10, 0

section .text
global digit_ops       ; Делаем функцию видимой для C
extern printf          ; Используем printf из libc

digit_ops:             ; int digit_ops(int number)
    push ebp           ; Пролог функции (сохраняем ebp)
    mov ebp, esp

    mov eax, [ebp + 8] ; Берём аргумент (number)
    xor ebx, ebx       ; sum = 0
    mov ecx, 1         ; product = 1

calculate_loop:
    test eax, eax
    jz done

    xor edx, edx
    mov esi, 10
    div esi            ; edx = digit, eax = num / 10

    add ebx, edx       ; sum += digit
    imul ecx, edx      ; product *= digit

    jmp calculate_loop

done:
    ; Вычисляем модуль разности
    mov eax, ebx       ; eax = sum
    sub eax, ecx       ; eax = sum - product
    jns positive
    neg eax            ; если отрицательно, инвертируем

positive:
    ; Выводим результат через printf
    push eax           ; разность (3-й аргумент)
    push ecx           ; произведение (2-й аргумент)
    push ebx           ; сумма (1-й аргумент)
    push output_format ; форматная строка
    call printf
    add esp, 16        ; очищаем стек

    ; Возвращаем 0 (успех)
    xor eax, eax
    pop ebp            ; Эпилог функции
    ret

; --------------------------------------------------------------------- ;
; nasm -f elf32 digit_ops.asm -o digit_ops.o  # Компилируем asm         ;
; gcc -m32 main.c digit_ops.o -o digit_ops    # Компилируем C + линкуем ;
; ./digit_ops                                 # Запускаем               ;
; --------------------------------------------------------------------- ;
