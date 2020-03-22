;=================================================
; _myPutc realization
;=================================================

section         .text

    global      _myPutc

;=================================================
; Prints one character
; Entry:    AL - character
;=================================================

_myPutc:        mov     [charToPrint], al
                push    rax
                push    rdi
                push    rsi
                push    rdx

                mov     rax, 1
                mov     rdi, 1
                mov     rsi, charToPrint
                mov     rdx, 1
                syscall

                pop     rdx
                pop     rsi
                pop     rdi
                pop     rax
                ret

section         .data

charToPrint     db  0

;=================================================
; End of _myPutc
;=================================================
