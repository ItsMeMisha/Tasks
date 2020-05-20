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
        mov     rcx, NumBuffer
        call    _utoa
        printNum
        jmp     formatEnd

%endmacro

%macro  printSnum 1

        mov     r8d, %1
        mov     rcx, NumBuffer
        call    _stoa
        printNum
        jmp     formatEnd

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
                jne     .letter
                call    Format
                jmp     .checkEnd
.letter:        mov     al, byte [rsi]
                call    _myPutc

.checkEnd:      inc     rsi
                cmp     byte [rsi], 0
                jne     .loop
           
                pop     r8                  ;end
                pop     rax
                pop     rdi
                pop     rsi
                leave
                ret

Format:         inc     rsi
                
                xor     rbx, rbx
                mov     bl, byte [rsi]
mov al, 'R'
                cmp     bl, '%'
                je      formatPerc
                cmp     bl, 'z'
                jae     Error
                cmp     bl, 'Z'
                jbe     .capitalLetter
                sub     bl, 'a' - 'A'
.capitalLetter: cmp     bl, 'B'
                jb      Error

                mov     rax, [rdi]
                jmp     [JmpTable + 8*rbx - 'B'*8]
                
;                caseESI '%', .formatPerc

;                caseESI 's', .formatS
;                caseESI 'S', .formatS
;                caseESI 'c', .formatC
;                caseESI 'C', .formatC
;                
;                caseESI 'd', .formatD
;                caseESI 'D', .formatD
;                caseESI 'i', .formatI
;                caseESI 'I', .formatI
;                caseESI 'u', .formatU
;                caseESI 'U', .formatU
;
;                
;                caseESI 'b', .formatB
;                caseESI 'B', .formatB
;                caseESI 'o', .formatO
;                caseESI 'O', .formatO
;                caseESI 'x', .formatX
;                caseESI 'X', .formatX

    formatEnd:      add     rdi, 8
                    ret

    formatS:        call    _myPuts
                    jmp     formatEnd

    formatC:        call    _myPutc
                    jmp     formatEnd

    formatPerc:     mov     al, '%'
                    call    _myPutc
                    jmp     formatEnd

    formatI:                           ;same as foramtD
    formatD:        printSnum 10d

    formatU:        printUnum 10d

    formatX:        printUnum 10h

    formatO:        printUnum 8d

    formatB:        printUnum 2d

    Error:          mov     rax, ErrorMsg
                    call    _myPuts
                    mov     al, bl
                    call    _myPutc
                    jmp     formatEnd

                ret

section         .data

ErrorMsg:       db 'Error, unkonown format ', 0

NumBuffer       times 65    db 0

JmpTable                        dq formatB
                                dq formatC
                                dq formatD
                times ('I'-'E') dq Error       ;symbols E - H
                                dq formatI
                times ('O'-'J') dq Error       ;symbols J - N
                                dq formatO
                times ('S'-'P') dq Error       ;symbols P - R
                                dq formatS
                                dq Error       ;symbol T
                                dq formatU
                times ('X'-'V') dq Error       ;symbols V, W
                                dq formatX
                times ('Z'-'X') dq Error       ;symbols Y, Z

;=================================================
; End of _myPrintf
;=================================================
