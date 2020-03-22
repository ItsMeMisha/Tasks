    extern  _myPrintf

    global  _start

section     .text

_start:     mov     rax, 242d
            push    rax
            push    rax
            push    rax
            push    rax

            mov     rdi, exmpl
            push    rdi
            mov     rdi, fmt
            push    rdi   
            call    _myPrintf

            mov rax, 60
            xor rdi, rdi
            syscall

section     .data

fmt         db  "hello, world!", 10, 13, "%s", 0

;, "%d", 10, 13, "%o", 10, 13, "%x", 10, 13, "%u", 10, 13, 0

exmpl       db  "I will sleep tonight!", 10, 13, 0
