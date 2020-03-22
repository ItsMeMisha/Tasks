;=================================================
; _myPrintf realization
;=================================================

%macro  caseESI 2

        cmp     byte [rsi], %1
        je      %2

%endmacro

%macro printNum 0

        mov     rax, NumBuffer
        call    _myPuts

%endmacro

%macro  printUnum 1

        mov     r8d, %1
        mov     ecx, NumBuffer
        call    _utoa
        printNum
        jmp     .formatEnd

%endmacro

%macro  printSnum 1

        mov     r8d, %1
        mov     ecx, NumBuffer
        call    _stoa
        printNum
        jmp     .formatEnd

%endmacro

section         .text

    extern  _myPutc
    extern  _myPuts
    extern  _stoa
    extern  _utoa

    global  _myPrintf

;=================================================
; C printf
;=================================================

_myPrintf:      enter   0, 0
                push    rsi
                push    rdi
                push    rax
                push    r8

                mov     rsi, [rbp + 16]      ;format string
                mov     rdi, rbp
                add     rdi, 24
                
.loop:         
                cmp     byte [rsi], '%'
                je      .format
                mov     al, byte [rsi]
                call    _myPutc

.checkEnd:      inc     rsi
                cmp     byte [rsi], 0
                jne     .loop
                jmp     .end

.format:        inc     rsi

                caseESI '%', .formatPerc
                mov     rax, [rdi]

                caseESI 's', .formatS
                caseESI 'S', .formatS
                caseESI 'c', .formatC
                caseESI 'C', .formatC
                
                caseESI 'd', .formatD
                caseESI 'D', .formatD
                caseESI 'i', .formatI
                caseESI 'I', .formatI
                caseESI 'u', .formatU
                caseESI 'U', .formatU

                caseESI 'o', .formatO
                caseESI 'O', .formatO
                caseESI 'x', .formatX
                caseESI 'X', .formatX

.formatEnd:     add     rdi, 8
                jmp     .checkEnd

.formatS:       call    _myPuts
                jmp     .formatEnd

.formatC:       call    _myPutc
                jmp     .formatEnd

.formatPerc:    mov     al, '%'
                call    _myPutc
                jmp     .formatEnd

.formatI:                           ;same as foramtD
.formatD:       printSnum 10d

.formatU:       printUnum 10d

.formatX:       printUnum 10h

.formatO:       printUnum 8d

.end:           

                pop     r8
                pop     rax
                pop     rdi
                pop     rsi
                leave
                ret

section         .data

NumBuffer       times 65 db 0

;=================================================
; End of _myPrintf
;=================================================
