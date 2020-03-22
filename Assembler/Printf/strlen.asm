;=================================================
; _myStrlen and _myMemchr realization
;=================================================

section     .text

    global  _myStrlen

;=================================================
; Counts length of the string
; Entry:    RDI - address of the string
;
; Ret:      EAX - length of the string
;=================================================


_myStrlen:      push    rdi
                push    rsi
                push    rcx

                xor     rax, rax
                xor     ecx, ecx
                xor     ecx, 0          ;max ecx
                mov     rsi, rdi
                
                call    _myMemchr

                cmp     rdi, 0
                je      .end

                sub     rdi, rsi
.end:           mov     rax, rdi

                pop     rcx
                pop     rsi
                pop     rdi
                ret

;=================================================
; End of the _myStrlen
;=================================================

    global  _myMemchr

;=================================================
; Finds position of the character in the byte string
; Entry:    EDI - address of the string
;           AL  - required character
;           ECX - length of the string
; Ret:      EDI - address of the character
;=================================================

_myMemchr:      cld
                repne   scasb
                jz     .found

                xor     rdi, rdi
                inc     rdi
.found:
                dec     rdi
                ret 

;=================================================
; End of the _myMemchr
;=================================================
