;=================================================
; _reverse realization
;=================================================

section         .text

    global      _reverse

;=================================================
; Reverses line from RCX to RDX
; Entry:    RCX - begin of the line
;           RDX - end of the line
;=================================================

_reverse:       push    rax
                push    rdx
                push    rcx

                cmp     rcx, rdx
                jae     .end

.loop:          mov     al, [rcx]
                mov     [rdx], al
                inc     rcx
                dec     rdx
                cmp     rcx, rdx
                jb      .loop
.end:

                pop     rcx
                pop     rdx
                pop     rax
                ret

;=================================================
; End of the _reverse
;=================================================

