;=================================================
; _reverse realization
;=================================================

section         .text

    global      _reverse

;=================================================
; Reverses line from RDX to RCX
; Entry:    RDX - begin of the line
;           RCX - end of the line
;=================================================

_reverse:       push    rax
                push    rdx
                push    rcx

                cmp     rdx, rcx
                jae     .end

.loop:          mov     al, byte [rdx]
                mov     ah, byte [rcx]
                mov     [rcx], al
                mov     [rdx], ah
                inc     rdx
                dec     rcx
                cmp     rdx, rcx
                jb      .loop
.end:

                pop     rcx
                pop     rdx
                pop     rax
                ret

;=================================================
; End of the _reverse
;=================================================

