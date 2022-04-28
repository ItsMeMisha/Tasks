# AsmAndCPU

Реализация эмулятора процессора и ассемблера для него.

Ассемблер поддерживает команды push, pop, add, sub, mul, div, end, in, out, regpush, jmp, ja, jae, jb, jbe, je, jne, call, ret, sqrrt.
Чтобы добвать новую команду ее достаточно описать в ([commands.h](commands.h)).

Примеры программ на этом ассемблере:
* [SquareEq.myasm](SquareEq.myasm)
* [fibbonachi.myasm](fibbonachi.myasm)
