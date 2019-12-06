
#define CHNG changes++;

#define NUM data.Ndata

#define ADD(LEFT, RIGHT) NewNode (type_Add, TFunc, LEFT, RIGHT) 
#define SUB(LEFT, RIGHT) NewNode (type_Sub, TFunc, LEFT, RIGHT)
#define MUL(LEFT, RIGHT) NewNode (type_Mul, TFunc, LEFT, RIGHT)
#define DIV(LEFT, RIGHT) NewNode (type_Div, TFunc, LEFT, RIGHT)
#define POW(LEFT, RIGHT) NewNode (type_Pow, TFunc, LEFT, RIGHT)
#define SIN(RIGHT) NewNode (type_Sin, TFunc, nullptr, RIGHT)
#define COS(RIGHT) NewNode (type_Cos, TFunc, nullptr, RIGHT)
#define EXP(RIGHT) NewNode (type_Exp, TFunc, nullptr, RIGHT)
#define LN(RIGHT)  NewNode (type_Ln,  TFunc, nullptr, RIGHT)
#define N(num)   NewNode (num, TNum, nullptr, nullptr)

#define BR branch
#define BRT branch -> type
#define BRL branch -> left
#define BRLT branch -> left -> type
#define BRR branch -> right
#define BRRT branch -> right -> type

#define DLT(node) DeleteBranch (OutTree, node);

#define DIF(In) NodeDiffer (In, OutTree, file)

#define SETF(Out, newdata, prior) Out -> data.Fdata = newdata; Out -> type = TFunc; Out -> priority = prior;
#define SETN(Out, newdata) Out -> data.Ndata = newdata; Out -> type = TNum; Out -> priority = 0;

#define CPYINOUT OutNode -> data = InNode -> data; OutNode -> type = InNode -> type; OutNode -> priority = InNode -> priority;
#define CPY(In, Out) CopyBranch (In, Out, OutTree)
#define CPOF(In) CopyOfBranch (In)
                     
#define IN InNode
#define INL InNode -> left
#define INLL InNode -> left -> left
#define INLR InNode -> left -> right
#define INR InNode -> right
#define INRL InNode -> right -> left
#define INRR InNode -> right -> right

#define OUT OutNode
#define OUTL OutNode -> left
#define OUTLL OutNode -> left -> left
#define OUTLR OutNode -> left -> right
#define OUTR OutNode -> right
#define OUTRL OutNode -> right -> left
#define OUTRR OutNode -> right -> right

#define NEWL(out) AddLeftLeaf (OutTree, out, {})
#define NEWR(out) AddRightLeaf (OutTree, out, {})

//NODE_TYPE (type_name, type_signature, type_num, priority, code, opimiseCode)

NODE_TYPE (NoType, " ", 0, 100,

    #ifdef DIFFER 

    {return nullptr;}

    #else   
    
    {}

    #endif

)

NODE_TYPE (Add, "+", 1, 3,

    #ifdef DIFFER 

{

    OUT = ADD (DIF (INL), DIF (INR));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if ((BRLT == TNum) && (BRRT == TNum)) {

        SETN (BR, BRL -> NUM + BRR -> NUM);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRLT == TNum && (BRL -> NUM == 0)) {

        Tree_node* tmp = BRL;

        CutBrunchWithoutLeaf (OutTree, BR, BRR);
        
        BR = tmp;

        CHNG;

    } else
 
    if (BRRT == TNum && (BRR -> NUM == 0)) {

        Tree_node* tmp = BRL;

        CutBrunchWithoutLeaf (OutTree, BR, BRL);
        
        BR = tmp;

        CHNG;

    }
        
}

    #else

    {}

    #endif
)

NODE_TYPE (Sub, "-", 2, 3,

    #ifdef DIFFER
{

    OUT = SUB (DIF (INL), DIF (INR));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRLT == TNum && BRRT == TNum) {

        SETN (BR, BRL -> NUM - BRR -> NUM);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRRT == TNum && (BRR -> NUM == 0)) {

        Tree_node* tmp = BRL;
        CutBrunchWithoutLeaf (OutTree, BR, BRL);        
        BR = tmp;

        CHNG;

    }
         
}

    #else

    {}

    #endif

)

NODE_TYPE (Mul, "*", 3, 2,

    #ifdef DIFFER

{

    OUT = ADD (MUL (DIF (INL), CPOF (INR)), MUL (CPOF (INL), DIF (INR)));
}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRLT == TNum && BRRT == TNum) {

        SETN (BR, (BRL -> NUM) * (BRR -> NUM));

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if  ((BRLT == TNum && BRL -> NUM == 0) || (BRRT == TNum && BRR -> NUM == 0)) {

        SETN (BR, 0);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRLT == TNum && BRL -> NUM == 1) {

        Tree_node* tmp = BRL;
        CutBrunchWithoutLeaf (OutTree, BR, BRR);        
        BR = tmp;

        CHNG;

    } else

    if (BRRT == TNum && BRR -> NUM == 1) {

        Tree_node* tmp = BRL;
        CutBrunchWithoutLeaf (OutTree, BR, BRL);        
        BR = tmp;

        CHNG;

    }   
        
}

    #else 

    {}

    #endif
)

NODE_TYPE (Div, "/", 4, 2,

    #ifdef DIFFER

{

    OUT = DIV (SUB (MUL (DIF (INL), CPOF (INR)), MUL (CPOF (INL), DIF (INR))), POW (CPOF (INR), N (2)));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRLT == TNum && BRRT == TNum) {

        SETN (BR, (BRL -> NUM) / (BRR -> NUM));

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRLT == TNum && BRL -> NUM == 0) {

        SETN (BR, 0);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRRT == TNum && (BRR -> NUM == 1)) {

        Tree_node* tmp = BRL;

        CutBrunchWithoutLeaf (OutTree, BR, BRL);

        BR = tmp;

        CHNG;

    }   
 

}

    #else

    {}

    #endif

)

NODE_TYPE (Sin, "sin", 5, 1,


    #ifdef DIFFER

{

    OUT = MUL (DIF (INR), COS(CPOF (INR)));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRRT == TNum) {

        SETN (BR, sin (BRR -> NUM));

        DLT (BRR);

        CHNG;

    }
        
}

    #else

    {}

    #endif

)

NODE_TYPE (Cos, "cos", 6, 1,

    #ifdef DIFFER

{

    OUT = MUL (N (-1), MUL (DIF (INR), SIN (CPOF (INR))));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRRT == TNum) {

        SETN (BR, cos (BRR -> NUM));

        DLT (BRR);

        CHNG;

    }
        
}

    #else

    {}

    #endif

)


NODE_TYPE (Tg, "tan", 7, 1,

    #ifdef DIFFER

{

    OUT = MUL (DIF (INR), DIV (N (1), POW (COS (CPOF (INR)), N (2))));

}

    #elif defined (OPIMISE)

{
    if (BRRT == TNum) {

        SETN (BR, tan (BRR -> NUM));
        DLT (BRR);
        CHNG;

    }

}

    #else

    {}

    #endif
    
)

NODE_TYPE (Ctg, "ctg", 8, 1,

    #ifdef DIFFER

{

    OUT = MUL (DIF (INR), DIV (N (-1), POW (SIN (CPOF (INR)), N (2))));

}

    #elif defined (OPIMISE)

{
    if (BRRT == TNum) {

        SETN (BR, 1/tan (BRR -> NUM));

        DLT (BRR);

        CHNG;

    }

}

    #else

    {}

    #endif
)

NODE_TYPE (Pow, "^", 9, 1,

    #ifdef DIFFER

{

    if (INL -> type != TNum && INR -> type == TNum) {


        OUT = MUL (MUL (N (INR -> NUM), DIF (INL)), POW (CPOF (INL), N (INR -> NUM - 1)));
    
    } else if (INL -> type == TNum) {

        OUT = MUL (MUL (LN (N (INL -> NUM)), DIF (INL)), CPOF (IN));

    }

    else {

        OUT = MUL (CPOF (IN), ADD (MUL (DIF (INR), LN (CPOF (INL))), DIV (MUL (DIF (INL), CPOF (INR)), CPOF (INL))));

    }

}

    #elif defined (OPTIMISE)

{   //OPTIMISE

    if (BRLT == TNum && BRRT == TNum) {

        SETN (BR, pow (BRL -> NUM, BRR -> NUM));

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRRT == TNum && (BRR -> NUM == 1)) {

        CutBrunchWithoutLeaf (OutTree, BR, BRL);

        CHNG;

    }   
 

}

    #else

    {}

    #endif

)

NODE_TYPE (Var, "x", 10, 0,

    #ifdef DIFFER

 {

    OUT = N (1);

}

    #else

    {}

    #endif
)

NODE_TYPE (Num, "0", 11, 0,

    #ifdef DIFFER

{

    OUT = N (0);

}

    #else

    {}

    #endif
)

NODE_TYPE (Exp, "exp", 12, 1,

    #ifdef DIFFER

{

        OUT = MUL (DIF (INR), CPOF (IN));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRRT == TNum) {

        SETN (BR, exp (BRR -> NUM));

        DLT (BRR);

        CHNG;

    }
}

    #else

    {}

    #endif

)

NODE_TYPE (Ln, "ln", 13, 1,

    #ifdef DIFFER

{

        OUT = DIV (DIF (INR), CPOF (INR));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRRT == TNum) {

        SETN (BR, log (BRR -> NUM));    

        DLT (BRR);

        CHNG;

    }

}

    #else

    {}

    #endif

)
NODE_TYPE (arcsin, "arcsin", 14, 1,

    #ifdef DIFFER

{

        OUT = DIV (DIF (INR), POW (SUB (N (1), POW (INR, N (2))), N (0.5)));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRRT == TNum) {

        SETN (BR, asin (BRR -> NUM));    

        DLT (BRR);

        CHNG;

    }

}

    #else

    {}

    #endif

)

NODE_TYPE (arccos, "arccos", 15, 1,

    #ifdef DIFFER

{

        OUT = DIV (MUL (N (-1), DIF (INR)), POW (SUB (N (1), POW (INR, N (2))), N (0.5)));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRRT == TNum) {

        SETN (BR, acos (BRR -> NUM));    

        DLT (BRR);

        CHNG;

    }

}

    #else

    {}

    #endif

)

NODE_TYPE (arctan, "arctan", 16, 1,

    #ifdef DIFFER

{

        OUT = DIV (DIF (INR), ADD (N (1), POW (INR, N (2))));

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRRT == TNum) {

        SETN (BR, atan (BRR -> NUM));    

        DLT (BRR);

        CHNG;

    }

}

    #else

    {}

    #endif

)


#undef SETF
#undef SETN
#undef CHNG
#undef NUM
#undef BR
#undef BRT
#undef BRL
#undef BRLT
#undef BRR
#undef BRRT
#undef DIF
#undef CPYINOUT
#undef CPY                     
#undef CPOF
#undef IN
#undef INL
#undef INLL
#undef INLR
#undef INR
#undef INRL
#undef INRR
#undef OUT
#undef OUTL
#undef OUTLL
#undef OUTLR
#undef OUTR
#undef OUTRL
#undef OUTRR
#undef NEWL
#undef NEWR
#undef DLT

