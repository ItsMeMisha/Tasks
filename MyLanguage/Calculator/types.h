
#define CHNG changes++;

#define BR branch

#define BRL branch -> left

#define BRR branch -> right

#define DLT(node) DeleteBranch (OutTree, node);

#define DIF(In, Out) NodeDiffer (In, Out, OutTree)

#define SET(Out, newdata, newtype, prior) Out -> data.Fdata = newdata; Out -> type = newtype; Out -> priority = prior;

#define SETN(Out, newdata) Out -> data.Ndata = newdata; Out -> type = TNum; Out -> priority = 0;

#define CPYINOUT OutNode -> data = InNode -> data; OutNode -> type = InNode -> type; OutNode -> priority = InNode -> priority;

#define CPY(In, Out) CopyBranch (In, Out, OutTree)
                     
#define IN InNode

#define INL InNode -> left

#define INR InNode -> right

#define OUT OutNode

#define OUTL OutNode -> left

#define OUTR OutNode -> right

#define NEWL(out) AddLeftLeaf (OutTree, out, {})

#define NEWR(out) AddRightLeaf (OutTree, out, {})

//NODE_TYPE (type_name, type_signature, type_num, priority, code, opimiseCode)

NODE_TYPE (NoType, " ", 0, 100,

    #ifdef DIFFER 

    {return;}

    #else   
    
    {}

    #endif

)

NODE_TYPE (Add, "+", 1, 3,

    #ifdef DIFFER 

{

    CPYINOUT;
    NEWL (OUT);
    NEWR (OUT);
    DIF (INL, OUTL);
    DIF (INR, OUTR);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if ((BRL -> type == TNum) && (BRR -> type == TNum)) {

        double NBuf1 = BRL -> data.Ndata;
        double NBuf2 = BRR -> data.Ndata;

        SETN (BR, NBuf1+NBuf2);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRL -> type == TNum && (BRL -> data.Ndata == 0)) {

        Tree_node* tmp = BRL;

        CutBrunchWithoutLeaf (OutTree, BR, BRR);
        
        BR = tmp;

        CHNG;

    } else
 
    if (BRR -> type == TNum && (BRR -> data.Ndata == 0)) {

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

    CPYINOUT;
    NEWL (OUT);
    NEWR (OUT);
    DIF (INL, OUTL);
    DIF (INR, OUTR);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRL -> type == TNum && BRR -> type == TNum) {

        double NBuf1 = BRL -> data.Ndata;
        double NBuf2 = BRR -> data.Ndata;

        SETN (BR, NBuf1-NBuf2);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRR -> type == TNum && (BRR -> data.Ndata == 0)) {

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

    SET  (OUT, type_Add, TFunc, 3);

    NEWL (OUT);
    NEWR (OUT);

    CPY (IN, OUTL);
    CPY (IN, OUTR);   

    NEWL (OUTL);
    NEWR (OUTL);

    CPY (INR, OUTL -> right);
    DIF (INL, OUTL -> left);

    NEWL (OUTR);
    NEWR (OUTR);

    CPY (INL, OUTR -> left);
    DIF (INR, OUTR -> right);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRL -> type == TNum && BRR -> type == TNum) {

        double NBuf1 = BRL -> data.Ndata;
        double NBuf2 = BRR -> data.Ndata;

        SETN (BR, NBuf1*NBuf2);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if  ((BRL -> type == TNum && BRL -> data.Ndata == 0) || (BRR -> type == TNum && BRR -> data.Ndata == 0)) {

        SETN (BR, 0);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRL -> type == TNum && BRL -> data.Ndata == 1) {
        Tree_node* tmp = BRL;

        CutBrunchWithoutLeaf (OutTree, BR, BRR);
        
        BR = tmp;

        CHNG;

    } else

    if (BRR -> type == TNum && BRR -> data.Ndata == 1) {
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

    CPYINOUT;
    NEWL (OUT);
    NEWR (OUT);

    SET (OUTL, type_Sub, TFunc, 3);
    SET (OUTR, type_Pow, TFunc, 1);

    NEWL (OUTL);
    NEWR (OUTL);

    NEWL (OUTR);
    NEWR (OUTR);

    CPY (INR, OUTR -> left);
    SETN (OUTR -> right, 2);

    SET (OUTL -> left, type_Mul, TFunc, 2);
    SET (OUTL -> right, type_Mul, TFunc, 2);

    NEWL (OUTL -> left);
    NEWR (OUTL -> left);

    CPY (INR, OUTL -> left -> right);
    DIF (INL, OUTL -> left -> left);

    NEWL (OUTL -> right);
    NEWR (OUTL -> right);

    CPY (INL, OUTL -> right -> left);
    DIF (INR, OUTL -> right -> right);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRL -> type == TNum && BRR -> type == TNum) {

        double NBuf1 = BRL -> data.Ndata;
        double NBuf2 = BRR -> data.Ndata;

        SETN (BR, NBuf1/NBuf2);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRL -> type == TNum && BRL -> data.Ndata == 0) {

        SETN (BR, 0);

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRR -> type == TNum && (BRR -> data.Ndata == 1)) {

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

    SET (OUT, type_Mul, TFunc, 2);
    
    NEWL (OUT);
    NEWR (OUT);

    DIF (INR, OUTL);
    SET (OUTR, type_Cos, TFunc, 1);

    NEWR (OUTR);
    CPY (INR, OUTR -> right);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == TNum) {

        double NBuf1 = BRR -> data.Ndata;

        SETN (BR, sin (NBuf1));

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

    SET (OUT, type_Mul, TFunc, 2);
    
    NEWL (OUT);
    NEWR (OUT);

    NEWL (OUTL);
    NEWR (OUTL);

    SET (OUTL, type_Mul, TFunc, 2); 

    DIF (INR, OUTL -> left);
    SETN (OUTL -> right, -1);

    SET (OUTR, type_Sin, TFunc, 1);

    NEWR (OUTR);
    CPY (INR, OUTR -> right);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == TNum) {

        double NBuf1 = BRR -> data.Ndata;

        SETN (BR, cos (NBuf1));

        DLT (BRR);

        CHNG;

    }
        
}

    #else

    {}

    #endif

)


NODE_TYPE (Tg, "tg", 7, 1,

    #ifdef DIFFER

{
    SET (OUT, type_Div, TFunc, 2);
    NEWL (OUT);
    NEWR (OUT);

    SETN (OUTR, 1);
    SET (OUTL, type_Pow, TFunc, 1);

    NEWL (OUTL);
    NEWR (OUTL);

    SETN (OUTL -> right, 2);
    SET (OUTL -> left, type_Cos, TFunc, 1);

    NEWR (OUTL -> left);

    CPY (INR, OUTL -> left -> right);

}

    #elif defined (OPIMISE)

{
    if (BRR -> type == TNum) {

        double NBuf1 = BRR -> data.Ndata;

        SETN (BR, tan (NBuf1));

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
    SET (OUT, type_Div, TFunc, 2);
    NEWL (OUT);
    NEWR (OUT);

    SETN (OUTR, -1);
    SET (OUTL, type_Pow, TFunc, 1);

    NEWL (OUTL);
    NEWR (OUTL);

    SETN (OUTL -> right, 2);
    SET (OUTL -> left, type_Sin, TFunc, 1);

    NEWR (OUTL -> left);

    CPY (INR, OUTL -> left -> right);


}

    #elif defined (OPIMISE)

{
    if (BRR -> type == TNum) {

        double NBuf1 = BRR -> data.Ndata;

        SETN (BR, 1/tan (NBuf1));

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
    
        SET (OUT, type_Mul, TFunc, 2);

        NEWL (OUT);
        NEWR (OUT);

        SET (OUTL, type_Mul, TFunc, 2);

        NEWL (OUTR);
        NEWR (OUTR);

        NEWL (OUTL);
        NEWR (OUTL);

        SET (OUTR, type_Pow, TFunc, 1);

        CPY (INL, OUTR -> left);
        CPY (INR, OUTR -> right);
        CPY (INR, OUTL -> right);
        DIF (INL, OUTL -> left);
        
        double NBuf1 = OUTR -> right -> data.Ndata - 1;
        SETN (OUTR, NBuf1);

    } else {

        SET (OUT, type_Mul, TFunc, 2);

        NEWL (OUT);
        NEWR (OUT);

        CPY (IN, OUTL);
        
        SET (OUTR, type_Add, TFunc, 3);
        
        NEWL (OUTR);
        NEWR (OUTR);

        SET (OUTR -> right, type_Mul, TFunc, 2);
        SET (OUTR -> left, type_Div, TFunc, 2);

        NEWR (OUTR -> right);
        NEWL (OUTR -> right);

        DIF (INR, OUTR -> right -> left);
        SET (OUTR -> right -> right, type_Ln, TFunc, 1);

        NEWR (OUTR -> right -> right);
        CPY (INL, OUTR -> right -> right -> right);

        NEWL (OUTR -> left);
        SET (OUTR -> left -> left, type_Mul, TFunc, 2);
        
        NEWL (OUTR -> left -> left);
        NEWR (OUTR -> left -> left);

        DIF (INL, OUTR -> left -> left -> left);
        CPY (INR, OUTR -> left -> left -> right);

        NEWR (OUTR -> left);
        CPY (INL, OUTR -> left -> right);

    }

}

    #elif defined (OPTIMISE)

{   //OPTIMISE

    if (BRL -> type == TNum && BRR -> type == TNum) {

        double NBuf1 = BRL -> data.Ndata;
        double NBuf2 = BRR -> data.Ndata;

        SETN (BR, pow (NBuf1, NBuf2));

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRR -> type == TNum && (BRR -> data.Ndata == 1)) {

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

    SETN (OUT, 1);

}

    #else

    {}

    #endif
)

NODE_TYPE (Num, "0", 11, 0,

    #ifdef DIFFER

{

    SETN (OUT, 0);

}

    #else

    {}

    #endif
)

NODE_TYPE (Exp, "exp", 12, 1,

    #ifdef DIFFER

{

        SET (OUT, type_Mul, TFunc, 2);

        NEWL (OUT);
        NEWR (OUT);

        NEWL (OUTR);
        NEWL (OUTL);

        CPY (IN, OUTR);
        DIF (INR, OUTL);
 
}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == TNum) {

        double NBuf1 = BRR -> data.Ndata;

        SETN (BR, exp (NBuf1));

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

        SET (OUT, type_Div, TFunc, 2);

        NEWL (OUT);
        NEWR (OUT);

        CPY (INR, OUTR);
        DIF (INR, OUTL);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == TNum) {

        double NBuf1 = BRR -> data.Ndata;

        SETN (BR, log (NBuf1));    

        DLT (BRR);

        CHNG;

    }

}

    #else

    {}

    #endif

)

#undef SET
#undef SETN
#undef CHNG
#undef BR
#undef BRL
#undef BRR
#undef DIF
#undef CPYINOUT
#undef CPY                     
#undef IN
#undef INL
#undef INR
#undef OUT
#undef OUTL
#undef OUTR
#undef NEWL
#undef NEWR
#undef DLT

