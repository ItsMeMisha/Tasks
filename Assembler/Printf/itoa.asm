;=================================================
; _utoa and _stoa realiztion
;=================================================

section     .text

    extern  _reverse

    global  _stoa

;=================================================
; Turns signed number into string (calls _utoa)
; Entry:    AX - number
;           EDI - address of the string buffer
;           DL - base
; Destr:    EDI
;=================================================

_stoa:          push    rcx
                mov     cx, 1
                shl     cx, 15
                and     cx, ax
                cmp     cx, 0
                je      .turn

                mov     byte [edi], '-'
                inc     edi
                xor     cx, cx
                dec     ax
                xor     ax, cx

.turn:          pop     rcx
                call    _utoa

                ret

;=================================================
; End of _stoa
;=================================================

    global  _utoa

;=================================================
; Turns signed number into string
; Entry:    AX - number
;           EDI - address of the string buffer
;           DL - base
; Destr:    EDI
;=================================================

_utoa:          push    rcx
                mov     ecx, edi

.loop:          div     dl
                mov     dh, ah
                mov     ah, byte [DIGIT+ebx]
                mov     byte [edi], ah
                inc     edi
                cmp     al, 0
                jne     .loop

                dec     edi
                call    _reverse
                pop     rcx

                ret

;=================================================
; End of _utoa
;=================================================


section         .data

DIGIT       db  '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ', 0
