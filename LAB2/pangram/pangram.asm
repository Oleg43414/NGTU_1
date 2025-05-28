section .data
    input_string db "The quick brown fox jumps over the lazy dog", 0
    input_len equ $ - input_string - 1

    true_msg db 'true', 0
    true_len equ $ - true_msg
    false_msg db 'false', 0
    false_len equ $ - false_msg

section .bss
    seen_letters resd 1  ; 32-битная маска (26 бит используется)

section .text
    global _start

_start:
    ; Инициализация битовой маски
    xor eax, eax
    mov [rel seen_letters], eax

    ; Настройка указателей
    lea rsi, [rel input_string]
    mov ecx, input_len

process_char:
    ; Загрузка символа с нулевым расширением
    movzx eax, byte [rsi]

    ; Проверка строчных букв (a-z)
    cmp al, 'a'
    jb check_upper
    cmp al, 'z'
    ja next_char
    sub al, 'a'          ; Преобразование в индекс 0-25
    jmp mark_seen

check_upper:
    ; Проверка заглавных букв (A-Z)
    cmp al, 'A'
    jb next_char
    cmp al, 'Z'
    ja next_char
    sub al, 'A'          ; Преобразование в индекс 0-25

mark_seen:
    ; Установка соответствующего бита
    mov edx, 1
    mov cl, al           ; Используем CL для сдвига
    shl edx, cl
    or [rel seen_letters], edx

next_char:
    ; Переход к следующему символу
    inc rsi
    dec ecx
    jnz process_char

check_result:
    ; Проверка всех 26 битов
    mov eax, [rel seen_letters]
    and eax, 0x03FFFFFF  ; Маска для 26 букв
    cmp eax, 0x03FFFFFF
    je is_pangram

not_pangram:
    ; Вывод "false" (64-битный системный вызов)
    mov rax, 1           ; sys_write
    mov rdi, 1           ; stdout
    lea rsi, [rel false_msg]
    mov rdx, false_len
    syscall
    jmp exit

is_pangram:
    ; Вывод "true" (64-битный системный вызов)
    mov rax, 1           ; sys_write
    mov rdi, 1           ; stdout
    lea rsi, [rel true_msg]
    mov rdx, true_len
    syscall

exit:
    ; Завершение программы
    mov rax, 60          ; sys_exit
    xor rdi, rdi         ; код возврата 0
    syscall

; -------------------------------------- ;
; nasm -f elf64 pangram.asm -o pangram.o ;
; ld -o pangram pangram.o                ;
; ./pangram                              ;
; -------------------------------------- ;
