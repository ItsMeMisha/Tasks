;=================================================
; _utoa and _stoa realiztion
;=================================================

section     .text

    extern  _reverse

    global  _stoa

;=================================================
; Turns signed number into string (calls _utoa)
; Entry:    EAX - number
;           ECX - address of the string buffer
;           r8w - base
;=================================================

_stoa:          push    rcx
                push    r9

                mov     r9, 1
                shl     r9, 63
                and     r9, rax
                cmp     r9, 0
                je      .turn

                mov     byte [rcx], '-'
                inc     rcx
                xor     r9, r9
                not     r9
                dec     rax
                xor     rax, r9

.turn:          call    _utoa
                pop     r9
                pop     rcx    

                ret

;=================================================
; End of _stoa
;=================================================

    global  _utoa

;=================================================
; Turns signed number into string
; Entry:    EAX - number
;           ECX - address of the string buffer
;           r8d - base
;=================================================

_utoa:          push    rcx
                push    rdx
                push    r9

                mov     r9, rcx

.loop:          xor     edx, edx
                div     r8d
                mov     ebx, edx
                mov     dh, byte [DIGIT+rbx]
                mov     [rcx], dh
                inc     rcx
                cmp     eax, 0
                jne     .loop
                
                mov     byte [rcx], 0
                dec     rcx
                mov     rdx, r9
                call    _reverse

                pop     r9
                pop     rdx
                pop     rcx

                ret

;=================================================
; End of _utoa
;=================================================


section         .data

DIGIT       db  '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', 0
