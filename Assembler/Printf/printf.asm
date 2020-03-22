;=================================================
; _myPrintf realization
;=================================================

%macro  caseESI 2

        cmp     [esi], %1
        je      %2

%endmacro

%macro printNum

        mov     eax, NumBuffer
        call    _myPuts

%endmacro

%macro  printUnum 1

        mov     ?, %1
        mov     ?, NumBuffer
        call    _utoa
        printNum
        jmp     .formatEnd

%endmacro

%macro  printSnum 1

        mov     ?, %1
        mov     ?, NumBuffer
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
                mov     rsi, [ebp + 16]      ;format string
                mov     edi, ebp
                add     edi, 24
                
.loop:         
                cmp     [esi], '%'
                je      .format
                mov     al, byte [esi]
                call    _myPutc

.checkEnd:      inc     esi
                cmp     [esi], 0
                jne     .loop
                jmp     .end

.format:        inc     esi

                case    '%' .formatPerc
                mov     rax, [edi]

                case    's' .formatS
                case    'S' .formatS
                case    'c' .formatC
                case    'C' .formatC
                
                case    'd' .formatD
                case    'D' .formatD
                case    'i' .formatI
                case    'I' .formatI
                case    'u' .formatU
                case    'U' .formatU

                case    'o' .formatO
                case    'O' .formatO
                case    'x' .formatX
                case    'X' .formatX

.formatEnd:     add     edi, 8
                inc     esi
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

.end:           leave
                ret

section         .data

NumBuffer       times 65 db 0

;=================================================
; End of _myPrintf
;=================================================
