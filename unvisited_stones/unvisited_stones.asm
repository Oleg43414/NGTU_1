section .data
    visited times 100 db 0  ; Максимум 100 валунов

section .text
global unvisited_stones

; ------------------------------------------------------------- ;
; Находит количество валунов, которые не посетит ни одна птица. ;
;                                                               ;
; @param [ebp+8]  N (количество птиц)                           ;
; @param [ebp+12] stones (общее количество валунов)             ;
; @param [ebp+16] birds (указатель на массив дистанций птиц)    ;
; @return eax (количество непосещённых валунов)                 ;
; ------------------------------------------------------------- ;

unvisited_stones:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi

    ; Очистка массива visited
    mov ecx, [ebp+12]       ; stones
    lea edi, [visited + 1]  ; visited[1..stones]
    xor eax, eax
    clear_loop:
        mov byte [edi + eax], 0
        inc eax
        cmp eax, ecx
        jl clear_loop

    ; Отметка посещённых валунов
    mov esi, [ebp+16]       ; birds array
    mov ecx, [ebp+8]        ; N (количество птиц)
    bird_loop:
        mov ebx, [esi]      ; bird distance
        mov edx, ebx        ; pos = bird
        stone_loop:
            cmp edx, [ebp+12]
            jg stone_done
            mov byte [edi + edx - 1], 1  ; visited[pos] = true
            add edx, ebx
            jmp stone_loop
        stone_done:
        add esi, 4          ; следующий элемент birds
        loop bird_loop

    ; Подсчёт непосещённых валунов
    xor eax, eax
    mov ecx, [ebp+12]
    count_loop:
        cmp byte [edi + ecx - 1], 0
        jne not_unvisited
        inc eax
        not_unvisited:
        loop count_loop

    pop edi
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret

; ---------------------------------------------------------- ;
; [ Запуск и компиляция с NASM ]                             ;
; nasm -f elf32 unvisited_stones.asm -o unvisited_stones.o   ;
; gcc -m32 main.c unvisited_stones.o -o program && ./program ;
; ---------------------------------------------------------- ;
