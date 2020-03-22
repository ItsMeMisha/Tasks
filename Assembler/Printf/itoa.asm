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

                mov     r9w, 1
                shl     r9w, 15
                and     r9w, ax
                cmp     r9w, 0
                je      .turn

                mov     byte [rcx], '-'
                inc     ecx
                xor     r9w, r9w
                dec     ax
                xor     ax, r9w

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
