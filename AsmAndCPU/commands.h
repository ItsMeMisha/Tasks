#define PUSH(element) StackPush(&Stack, element)

#define POP StackPop(&Stack)

#define NEXT(shift) current += shift

#define jumpComp(sign)                      \
                                            \
    NEXT (1);                               \
    Element_t firstElem = POP;              \
    Element_t secondElem = POP;             \
                                            \
    if (firstElem sign secondElem)          \
        current = *((int*) (cmd + current));\
                                            \
    else NEXT (sizeof (int));


// DEF_CMD (name, cmdNum, numOfArg, codeForCpu)

// push, pop, add, sub, mul, div, end, in, out, regpush,
// jmp, ja, jae, jb, jbe, je, jne, call, ret, sqrrt
 

DEF_CMD (push, 1, 1, {

    NEXT (1);
    PUSH (*((int*) (cmd +current)));

    NEXT (sizeof (Element_t)); 

    }
)

DEF_CMD (pop, 2, 1, {

    NEXT (1);
    regstr[cmd[current]] = POP;
    NEXT (1);

    }
)

DEF_CMD (add, 3, 0, {

    PUSH (POP + POP);

    NEXT (1);
    }
)

DEF_CMD (sub, 4, 0, {

    Element_t firstelem = POP;
    PUSH (POP - firstelem);
    NEXT (1);

    }
)

DEF_CMD (mul, 5, 0, {

    PUSH (POP * POP / Accuracy);
    NEXT (1);

    }
)

DEF_CMD (div, 6, 0, {

    Element_t buf = POP;
    PUSH ((int) ( (double) POP / (double) buf * Accuracy));
    NEXT (1);

    }
)

DEF_CMD (end, 7, 0, {

    StackDestruct (&Stack);
    EndOfProgram = true;

    }
)

DEF_CMD (in, 8, 0, {

    double buf = 0;
    scanf ("%lg", &buf);
    PUSH (static_cast <int> (buf * Accuracy));
    NEXT (1);

    }
)

DEF_CMD (out, 9, 0, {

    Element_t buf = POP;
    printf ("%g\n", (double) buf / Accuracy);
    NEXT (1);

    }
)

DEF_CMD (regpush, 10, 1, {

    NEXT (1);
    PUSH (regstr[cmd[current]]);
    NEXT (1);

    }
)
DEF_CMD (jmp, 11, 1, {

    NEXT (1);
    current = *((int*) (cmd + current));

    }
)    

DEF_CMD (ja, 12, 1, {

    jumpComp (>);

    }
)

DEF_CMD (jae, 13, 1, {

    jumpComp (>=);

    }
)

DEF_CMD (jb, 14, 1, {

    jumpComp (<);

    }
)

DEF_CMD (jbe, 15, 1, {
    
    jumpComp (<=);

    }
)

DEF_CMD (je, 16, 1, {

    jumpComp (==);

    }
)

DEF_CMD (jne, 17, 1, {

    jumpComp (!=);

    }
)

DEF_CMD (call, 18, 1, {

    NEXT (1);
    PUSH (current + sizeof (int));
    current = *((int*) (cmd + current));

    }
)

DEF_CMD (ret, 19, 0, {

    current = POP;

    }
)

DEF_CMD (sqrrt, 20, 0, {

    PUSH (sqrt (POP) * sqrt (Accuracy));
    NEXT (1);

    }
)

#undef PUSH
#undef POP
#undef NEXT
#undef jumpComp
