;=================================================
; _reverse realization
;=================================================

section         .text

    global      _reverse

;=================================================
; Reverses line from ECX to EDX
; Entry:    ECX - begin of the line
;           EDX - end of the line
; Destr     ECX, EDX
;=================================================

_reverse:       push    rax

                cmp     ecx, edi
                jae     .end

.loop:          mov     al, [ecx]
                mov     [edi], al
                inc     ecx
                dec     edi
                cmp     ecx, edi
                jb      .loop
.end:
                pop     rax
                ret

;=================================================
; End of the _reverse
;=================================================

