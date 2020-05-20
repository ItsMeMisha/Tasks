;=================================================
; _binRadixToa realization
;=================================================

section         .text

    extern  _reverse

    global  _binRadixToa

;=================================================
; Converses number to a string with a degree of 2 base
; Entry:    EAX - number
;           RCX - address of the string buffer
;           r8b - degree of the base
;=================================================

_binRadixToa:   push    rcx
                push    rdi
                push    rdx
                mov     rdi, rcx
                mov     r9, rdi
                mov     cl, r8b
                xor     edx, edx

.loop:          mov     edx, eax
                shr     eax, cl
                shl     eax, cl
                xor     edx, eax
                shr     eax, cl

                mov     ebx, edx
                mov     dh, byte [DIGIT+rbx]
                mov     [rdi], dh
                inc     rdi
                cmp     eax, 0
                jne     .loop

                mov     byte [rdi], 0
                dec     rdi
                mov     rcx, rdi
                mov     rdx, r9
                call    _reverse
                pop     rdx
                pop     rdi
                pop     rcx

                ret

;=================================================
; End of _binRadixToa
;=================================================

section         .data

DIGIT       db  '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', 0
