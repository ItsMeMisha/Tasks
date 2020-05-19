section .data

    gretting: db 'What must you say while invading secret trade federation base', 0 ;??????
    passBuf times 10h db 0     ;pass is "hellothere" - 10 symbols

    wrongPass: db 'No. Impossible. Perhaps, the archives are incomplete', 10, 0 ; 53 symbols
    wrongPhraseLength   dq 52

    rightPass: db 'You are in. You hear: "General Kenobi..."', 10, 0 ; 42 symbols
    rightPhraseLength   dq 42

section .text

global _start
_start:
        call    readPass
        call    checkHash
        call    close

readPass:
        mov     rax, 0
        mov     rdi, 0  ;stdin
        mov     rsi, passBuf
        mov     rdx, 10
        syscall
        ret

checkHash:   

        mov     rdi, passBuf
        mov     rcx, 10
        call    countHash
    
        cmp     rax, 1105987815d      ;hash
        jne     .wrong
        mov     rsi, rightPass
        mov     rdx, [rightPhraseLength]
        call    print
        ret
.wrong:
        mov     rsi, wrongPass
        mov     rdx, [wrongPhraseLength]
        call    print
        ret

countHash:
        
        xor     rbx, rbx
        xor     rax, rax

.hashLoop:
        lodsb
        mul     rcx
        add     rbx, rax
        loop .hashLoop

        mov     rax, rbx
        ret

print:
        mov     rax, 1
        mov     rdi, 1
        syscall
        ret

close:
        mov     rax, 60
        xor     rdi, rdi
        syscall
