asm.out: asm.cpp
	g++ asm.cpp -o asm.out

SquareEq.myasm: SquareEq.myasm asm.out
	./asm.out SquareEq.myasm SquareEq.myexe

cpu.out: cpu.cpp
	g++ cpu.cpp -o cpu.out

SquareEq.myexe: SquareEq.myasm SquareEq.myexe asm.out cpu.out
	./asm.out SquareEq.myasm SquareEq.myexe
	./cpu.out SquareEq.myexe
