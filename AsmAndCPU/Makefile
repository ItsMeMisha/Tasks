asm.out: asm.cpp
    g++ asm.cpp -o asm.out

SquareEq.myexe: SquareEq.myasm asm.out
    ./asm.out SquareEq.myasm SquareEq.myexe

cpu.out: cpu.cpp
    g++ cpu.cpp -o cpu.out

SquareEq.exed: SquareEq.myexe cpu.out
    ./cpu.out SquareEq.myexe SquareEq.exed
