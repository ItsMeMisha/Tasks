#define PUSH(element) StackPush(Stack, element)

#define POP StackPop(Stack)

#define NEXT(shift) current += shift

#define IsNumberParam (cmd[commandNum] & NumberparamMask)

#define IsRegisterParam (cmd[commandNum] & RegisterparamMask)

#define IsRamParam (cmd[commandNum] & RamparamMask)

#define NumRead *((int*) (cmd + current))

#define ReadNumParam                \
                                    \
    if (IsNumberParam) {            \
                                    \
        tmpValue = NumRead;         \
        NEXT (sizeof (Element_t));  \
                                    \
    }

#define ReadRegParam                        \
                                            \
    if (IsRegisterParam){                   \
                                            \
        tmpValue += regstr[cmd[current]];   \
        sleep (0.1);                        \
        NEXT (1);                           \
                                            \
    }



#define CheckLabel                                       \
    if (*((int*)(cmd + current)) == 0) {                 \
        printf ("undefined label at %x\n", current - 1); \
        return 3;                                        \
        }                                   
        

#define jumpComp(sign)                      \
                                            \
    NEXT (1);                               \
    Element_t firstElem = POP;              \
    Element_t secondElem = POP;             \
                                            \
    if (firstElem sign secondElem) {        \
        CheckLabel;                         \
        current = *((int*) (cmd + current));\
    }                                       \
                                            \
    else NEXT (sizeof (int));

// DEF_CMD (name, cmdNum, numOfArg, codeForCpu)

// push, pop, add, sub, mul, div, end, in, out, regpush,
// jmp, ja, jae, jb, jbe, je, jne, call, ret, sqrrt
 

DEF_CMD (push, 1, 1, {

    int commandNum = current;

    NEXT (1);
    
    int tmpValue = 0;
    
    ReadNumParam;
    ReadRegParam;

    if ((IsRamParam) && (tmpValue >= 0) && (tmpValue < RAMSize * Accuracy)) {

        sleep (1);
        PUSH (RAM[tmpValue / Accuracy]);

    }

    else if (((tmpValue < 0) || (tmpValue >= RAMSize * Accuracy)) && IsRamParam) {

        printf ("Invalid RAM address\n");
        return 2;

    }
    
    else if (IsRamParam == 0) 
        PUSH (tmpValue);
    
}
)

DEF_CMD (pop, 2, 1, {

    int commandNum = current;

    NEXT (1);

    if ((IsRegisterParam) && (IsRamParam == 0)) {

        regstr[cmd[current]] = POP;
        NEXT (1);

    }

    else if (IsRamParam) {

        int tmpValue = 0;
        
        ReadNumParam;
        ReadRegParam;

        if ((tmpValue >= 0) && (tmpValue < RAMSize * Accuracy)) {

            sleep (1);
            RAM[tmpValue / Accuracy] = POP;

        }

        else {

            printf ("Invalid RAM address\n");
            return 2;

        }

    }

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

    if (buf == 0) {
        
        printf ("Division by zero\n");
        return 3;

    }

    PUSH ((int) ( (double) POP / (double) buf * Accuracy));
    NEXT (1);

    }
)

DEF_CMD (end, 7, 0, {

    StackDestruct (Stack);
    EndOfProgram = true;

    }
)

DEF_CMD (in, 8, 0, {

    double buf = 0;
    scanf ("%lg", &buf);
    PUSH ((int) (buf * Accuracy));
    NEXT (1);

    }
)

DEF_CMD (out, 9, 0, {

    Element_t buf = POP;
    printf ("%lg\n", (double) buf / Accuracy);
    NEXT (1);

    }
)

DEF_CMD (regpush, 10, 1, {

    NEXT (1);
    sleep (0.1);
    PUSH (regstr[cmd[current]]);
    NEXT (1);

    }
)

DEF_CMD (jmp, 11, 1, {

    NEXT (1);
    CheckLabel;
    current = NumRead;

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
    CheckLabel;
    PUSH (current + sizeof (int));
    current = NumRead;

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

DEF_CMD (draw, 21, 0, {

    DrawCanv (VidMem, color);
    NEXT (1);

    }
)

DEF_CMD (reflex, 22, 0, {

    VidReflex (VidMem);
    sleep (0.1);
    NEXT (1);

    }
)

#undef PUSH
#undef POP
#undef NEXT
#undef IsNumberParam 
#undef IsRegisterParam
#undef IsRamParam 
#undef NumRead
#undef CheckLabel
#undef jumpComp
