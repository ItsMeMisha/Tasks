
#define CHNG changes++;

#define BR branch

#define BRL branch -> left

#define BRR branch -> right

#define DLT(node) DeleteBranch (OutTree, node);

#define DIF(In, Out) NodeDiffer (In, Out, OutTree)

#define SET(Out, newdata, newtype, prior) sscanf (newdata, "%ms", &(Out -> data)); Out -> type = newtype; Out -> priority = prior;

#define CPYINOUT sscanf (InNode -> data, "%ms", &(OutNode -> data)); OutNode -> type = InNode -> type; OutNode -> priority = InNode -> priority;

#define CPY(In, Out) CopyBranch (In, Out, OutTree)
                     
#define IN InNode

#define INL InNode -> left

#define INR InNode -> right

#define OUT OutNode

#define OUTL OutNode -> left

#define OUTR OutNode -> right

#define NEWL(out) AddLeftLeaf (OutTree, out, 0)

#define NEWR(out) AddRightLeaf (OutTree, out, 0)

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

    if ((BRL -> type == type_Num) && (BRR -> type == type_Num)) {

        double NBuf1 = atof (BRL -> data);
        double NBuf2 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", NBuf1 + NBuf2);

        BR -> type = type_Num;
        BR -> priority = 0;

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRL -> type == type_Num && (atof (BRL -> data) == 0)) {

        Tree_node* tmp = BRL;

        CutBrunchWithoutLeaf (OutTree, BR, BRR);
        
        BR = tmp;

        CHNG;

    } else
 
    if (BRR -> type == type_Num && (atof (BRR -> data) == 0)) {

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

    if (BRL -> type == type_Num && BRR -> type == type_Num) {

        double NBuf1 = atof (BRL -> data);
        double NBuf2 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", NBuf1 - NBuf2);

        BR -> type = type_Num;
        BR -> priority = 0;

        DLT (BRL);
        DLT (BRR);


        CHNG;

    } else

    if (BRR -> type == type_Num && (atof (BRR -> data) == 0)) {

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

    SET (OUT, "+", type_Add, 3);
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

    if (BRL -> type == type_Num && BRR -> type == type_Num) {

        double NBuf1 = atof (BRL -> data);
        double NBuf2 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", NBuf1 * NBuf2);

        BR -> type = type_Num;
        BR -> priority = 0;

        DLT (BRL);
        DLT (BRR);


        CHNG;

    } else

    if  ((BRL -> type == type_Num && atof (BRL -> data) == 0) || (BRR -> type == type_Num && atof (BRR -> data) == 0)) {

        strncpy (BR -> data,  "0", 2);
        BR -> type = type_Num;
        BR -> priority = 0;

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRL -> type == type_Num && (atof (BRL -> data) == 1)) {
        Tree_node* tmp = BRL;

        CutBrunchWithoutLeaf (OutTree, BR, BRR);
        
        BR = tmp;

        CHNG;

    } else

    if (BRR -> type == type_Num && (atof (BRR -> data) == 1)) {
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

    SET (OUTL, "-", type_Sub, 3);
    SET (OUTR, "^", type_Pow, 1);

    NEWL (OUTL);
    NEWR (OUTL);

    NEWL (OUTR);
    NEWR (OUTR);

    CPY (INR, OUTR -> left);
    SET (OUTR -> right, "2", type_Num, 0);

    SET (OUTL -> left, "*", type_Mul, 2);
    SET (OUTL -> right, "*", type_Mul, 2);

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

    if (BRL -> type == type_Num && BRR -> type == type_Num) {

        double NBuf1 = atof (BRL -> data);
        double NBuf2 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", NBuf1 / NBuf2);

        BR -> type = type_Num;

        DLT (BRL);
        DLT (BRR);


        CHNG;

    } else

    if (BRL -> type == type_Num && (atof (BRL -> data) == 0)) {

        strncpy (BR -> data,  "0", 2);
        BR -> type = type_Num;
        BR -> priority = 0;

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRR -> type == type_Num && (atof (BRR -> data) == 1)) {

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

    SET (OUT, "*", type_Mul, 2);
    
    NEWL (OUT);
    NEWR (OUT);

    DIF (INR, OUTL);
    SET (OUTR, "cos", type_Cos, 1);

    NEWR (OUTR);
    CPY (INR, OUTR -> right);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == type_Num) {

        double NBuf1 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", sin (NBuf1));

        BR -> type = type_Num;

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

    SET (OUT, "*", type_Mul, 2);
    
    NEWL (OUT);
    NEWR (OUT);

    NEWL (OUTL);
    NEWR (OUTL);

    SET (OUTL, "*", type_Mul, 2); 

    DIF (INR, OUTL -> left);
    SET (OUTL -> right, "-1", type_Num, 0);

    SET (OUTR, "sin", type_Sin, 1);

    NEWR (OUTR);
    CPY (INR, OUTR -> right);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == type_Num) {

        double NBuf1 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", cos (NBuf1));

        BR -> type = type_Num;
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
    SET (OUT, "/", type_Div, 2);
    NEWL (OUT);
    NEWR (OUT);

    SET (OUTR, "1", type_Num, 0);
    SET (OUTL, "^", type_Pow, 1);

    NEWL (OUTL);
    NEWR (OUTL);

    SET (OUTL -> right, "2", type_Num, 0);
    SET (OUTL -> left, "cos", type_Cos, 1);

    NEWR (OUTL -> left);

    CPY (INR, OUTL -> left -> right);

}

    #elif defined (OPIMISE)

{
    if (BRR -> type == type_Num) {

        double NBuf1 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", tan (NBuf1));

        BR -> type = type_Num;
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
    SET (OUT, "/", type_Div, 2);
    NEWL (OUT);
    NEWR (OUT);

    SET (OUTR, "-1", type_Num, 0);
    SET (OUTL, "^", type_Pow, 1);

    NEWL (OUTL);
    NEWR (OUTL);

    SET (OUTL -> right, "2", type_Num, 0);
    SET (OUTL -> left, "sin", type_Sin, 1);

    NEWR (OUTL -> left);

    CPY (INR, OUTL -> left -> right);


}

    #elif defined (OPIMISE)

{
    if (BRR -> type == type_Num) {

        double NBuf1 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", 1/tan (NBuf1));

        BR -> type = type_Num;
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

    if (INL -> type != type_Num && INR -> type == type_Num) {
    
        SET (OUT, "*", type_Mul, 2);

        NEWL (OUT);
        NEWR (OUT);

        SET (OUTL, "*", type_Mul, 2);

        NEWL (OUTR);
        NEWR (OUTR);

        NEWL (OUTL);
        NEWR (OUTL);

        SET (OUTR, "^", type_Pow, 1);

        CPY (INL, OUTR -> left);
        CPY (INR, OUTR -> right);
        CPY (INR, OUTL -> right);
        DIF (INL, OUTL -> left);
        
        double NBuf1 = atof (OUTR -> right -> data) - 1;
        free (OUTR -> right -> data);
        OUTR -> right -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (OUTR -> right -> data, "%lg", NBuf1);

    } else {

        SET (OUT, "*", type_Mul, 2);

        NEWL (OUT);
        NEWR (OUT);

        CPY (IN, OUTL);
        
        SET (OUTR, "+", type_Add, 3);
        
        NEWL (OUTR);
        NEWR (OUTR);

        SET (OUTR -> right, "*", type_Mul, 2);
        SET (OUTR -> left, "/", type_Div, 2);

        NEWR (OUTR -> right);
        NEWL (OUTR -> right);

        DIF (INR, OUTR -> right -> left);
        SET (OUTR -> right -> right, "ln", type_Ln, 1);

        NEWR (OUTR -> right -> right);
        CPY (INL, OUTR -> right -> right -> right);

        NEWL (OUTR -> left);
        SET (OUTR -> left -> left, "*", type_Mul, 2);
        
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

    if (BRL -> type == type_Num && BRR -> type == type_Num) {

        double NBuf1 = atof (BRL -> data);
        double NBuf2 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", pow (NBuf1, NBuf2));

        BR -> type = type_Num;

        DLT (BRL);
        DLT (BRR);

        CHNG;

    } else

    if (BRR -> type == type_Num && (atof (BRR -> data) == 1)) {

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

    SET (OUT, "1", type_Num, 0);

}

    #else

    {}

    #endif
)

NODE_TYPE (Num, "0", 11, 0,

    #ifdef DIFFER

{

    SET (OUT, "0", type_Num, 0);

}

    #else

    {}

    #endif
)

NODE_TYPE (Exp, "exp", 12, 1,

    #ifdef DIFFER

{

        SET (OUT, "*", type_Mul, 2);

        NEWL (OUT);
        NEWR (OUT);

        NEWL (OUTR);
        NEWL (OUTL);

        CPY (IN, OUTR);
        DIF (INR, OUTL);
 
}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == type_Num) {

        double NBuf1 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", exp (NBuf1));

        BR -> type = type_Num;
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

        SET (OUT, "/", type_Div, 2);

        NEWL (OUT);
        NEWR (OUT);

        CPY (INR, OUTR);
        DIF (INR, OUTL);

}

    #elif defined (OPTIMISE)

{ //OPTIMISE

    if (BRR -> type == type_Num) {

        double NBuf1 = atof (BRR -> data);

        free (BR -> data);
        BR -> data = (char*) calloc (MaxStrBufSize, sizeof (char));
        sprintf (BR -> data, "%lg", log (NBuf1));

        BR -> type = type_Num;
        DLT (BRR);

        CHNG;

    }

}

    #else

    {}

    #endif

)

#undef CHNG
#undef BR
#undef BRL
#undef BRR
#undef DIF
#undef SET
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

