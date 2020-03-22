;=================================================
; _myStrlen and _myMemchr realization
;=================================================

section     .text

    global  _myStrlen

;=================================================
; Counts length of the string
; Entry:    EDI - address of the string
;
; Ret:      EAX - length of the string
;=================================================


_myStrlen:      push    rdi
                push    rsi
                push    rcx

                xor     al, al
                xor     ecx, ecx
                xor     ecx, 0          ;max ecx
                mov     esi, edi
                
                call    _myMemchr

                sub     edi, esi
                mov     eax, edi

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
                je     .found

                xor     edi, edi
.found:
                dec     edi
                ret 

;=================================================
; End of the _myMemchr
;=================================================
