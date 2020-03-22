;=================================================
; _myPuts realization
;================================================

section     .text

    extern  _myStrlen

    global  _myPuts

;=================================================
; Puts string to stdout
; Entry:    EDI - address of the string
;=================================================

_myPuts:        push    rax
                push    rdi
                push    rsi
                push    rdx

                call    _myStrlen 
                mov     edx, eax
                mov     esi, edi
                mov     rdi, 1
                mov     rax, 1
                syscall 

                pop     rdx
                pop     rsi
                pop     rdi
                pop     rax                
                ret

;=================================================
; End of _myPuts
;=================================================
