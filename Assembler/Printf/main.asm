    extern  _myPrintf

    global  _start

section     .text

_start: 

;=======================================
;TEST1
;=======================================

            push    127d
            push    '!'
            push    100d
            push    3802d
            mov     rsi, lovestr
            push    rsi
            mov     rdi, test1
            push    rdi   
            call    _myPrintf

;=======================================
;TEST2
;=======================================

            push    -8086d
            push    -8086d
            mov     rdi, test2
            push    rdi   
            call    _myPrintf
;=======================================
;TEST3
;=======================================

            push    8086d
            push    8086d
            mov     rdi, test3
            push    rdi   
            call    _myPrintf
;=======================================
;TEST4
;=======================================

            push    8086d
            mov     rdi, test4
            push    rdi   
            call    _myPrintf
;=======================================
;End of tests
;=======================================

            mov rax, 60
            xor rdi, rdi
            syscall

section     .data

test1       db "I %s %x %d%%%c%b", 10, 13, 0
lovestr     db 'love', 0

test2       db "Test2: %%d:%d, %%u:%u", 10, 13, 0
test3       db "Test3: %%x:%x, %%o:%o", 10, 13, 0
test4       db "Test4: %%b:%b", 10, 13, 0


