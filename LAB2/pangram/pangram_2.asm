section .data
    ; Тестовая строка (можно изменить для проверки)
    input_string db "The quick brown fox jumps over the lazy dog", 0
    input_len equ $ - input_string - 1  ; длина без нуль-терминатора

    ; Сообщения вывода
    true_msg db 'true', 0
    true_len equ $ - true_msg
    false_msg db 'false', 0
    false_len equ $ - false_msg

section .bss
    ; 32-битная маска для отметки букв (a-z)
    seen_letters resd 1

section .text
    global _start

; ================================================ ;
; Основная функция проверки панграммы              ;
; Алгоритм:                                        ;
; 1. Инициализирует битовую маску                  ;
; 2. Обрабатывает каждый символ строки             ;
; 3. Отмечает найденные буквы в битовой маске      ;
; 4. Проверяет, все ли 26 букв найдены             ;
; ================================================ ;
_start:
    ; Обнуление битовой маски
    mov dword [seen_letters], 0

    ; Настройка указателя и счетчика
    mov rsi, input_string    ; Указатель на строку
    mov ecx, input_len       ; Счетчик символов

process_loop:
    ; Загрузка символа
    movzx eax, byte [rsi]    ; Загружаем символ с расширением нулем

    ; Проверка строчных букв (a-z)
    cmp al, 'a'
    jb .check_upper
    cmp al, 'z'
    ja .next_char
    sub al, 'a'              ; Преобразуем в индекс 0-25
    jmp .set_bit

.check_upper:
    ; Проверка заглавных букв (A-Z)
    cmp al, 'A'
    jb .next_char
    cmp al, 'Z'
    ja .next_char
    sub al, 'A'              ; Преобразуем в индекс 0-25

.set_bit:
    ; Установка соответствующего бита
    mov edx, 1
    mov cl, al              ; Используем CL для сдвига
    shl edx, cl             ; Сдвигаем бит
    or [seen_letters], edx  ; Устанавливаем бит в маске

.next_char:
    ; Переход к следующему символу
    inc rsi
    dec ecx
    jnz process_loop        ; Продолжаем пока ECX ≠ 0

check_result:
    ; Проверяем все 26 битов (0x03FFFFFF)
    mov eax, [seen_letters]
    and eax, 0x03FFFFFF     ; Маска для 26 битов
    cmp eax, 0x03FFFFFF
    je .true_case

.false_case:
    ; Вывод "false"
    mov eax, 4              ; sys_write
    mov ebx, 1              ; stdout
    mov ecx, false_msg
    mov edx, false_len
    int 0x80
    jmp .exit

.true_case:
    ; Вывод "true"
    mov eax, 4              ; sys_write
    mov ebx, 1              ; stdout
    mov ecx, true_msg
    mov edx, true_len
    int 0x80

.exit:
    ; Завершение программы
    mov eax, 1              ; sys_exit
    xor ebx, ebx            ; код возврата 0
    int 0x80

; ------------------------- ;
; nasm -f elf64 pangram.asm ;
; ld -o pangram pangram.o   ;
; ./pangram                 ;
; ------------------------- ;
