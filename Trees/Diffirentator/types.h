
#define DIF(In, Out) NodeDiffer (In, Out, OutTree)

#define SET(Out, newdata, newtype) sscanf (newdata, "%ms", &(Out -> data)); Out -> type = newtype;

#define CPYINOUT sscanf (InNode -> data, "%ms", &(OutNode -> data)); OutNode -> type = InNode -> type;

#define CPY(In, Out) CopyBranch (In, Out, OutTree)
                     
#define IN InNode

#define INL InNode -> left

#define INR InNode -> right

#define OUT OutNode

#define OUTL OutNode -> left

#define OUTR OutNode -> right

#define NEWL(out) AddLeftLeaf (OutTree, out, 0)

#define NEWR(out) AddRightLeaf (OutTree, out, 0)

//NODE_TYPE (type_name, type_signature, type_num, code)

NODE_TYPE (NoType, " ", 0, return;  )

NODE_TYPE (Add, "+", 1, {

    CPYINOUT;
    NEWL (OUT);
    NEWR (OUT);
    DIF (INL, OUTL);
    DIF (INR, OUTR);

}
)

NODE_TYPE (Sub, "-", 2, {

    CPYINOUT;
    NEWL (OUT);
    NEWR (OUT);
    DIF (INL, OUTL);
    DIF (INR, OUTR);

}
)

NODE_TYPE (Mul, "*", 3, {

    SET (OUT, "+", type_Add);
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
)

NODE_TYPE (Div, "/", 4, {

    CPYINOUT;
    NEWL (OUT);
    NEWR (OUT);

    SET (OUTL, "-", type_Sub);
    SET (OUTR, "^", type_Deg);

    NEWL (OUTL);
    NEWR (OUTL);

    NEWL (OUTR);
    NEWR (OUTR);

    CPY (INR, OUTR -> left);
    SET (OUTR -> right, "2", type_Num);

    SET (OUTL -> left, "*", type_Mul);
    SET (OUTL -> right, "*", type_Mul);

    NEWL (OUTL -> left);
    NEWR (OUTL -> left);

    CPY (INR, OUTL -> left -> right);
    DIF (INL, OUTL -> left -> left);

    NEWL (OUTL -> right);
    NEWR (OUTL -> right);

    CPY (INL, OUTL -> right -> left);
    DIF (INR, OUTL -> right -> right);

}
)

NODE_TYPE (Sin, "sin", 5, {




}
)

NODE_TYPE (Cos, "cos", 6, {



}
)

NODE_TYPE (Tg, "tg", 7, {



}
)

NODE_TYPE (Ctg, "ctg", 8, {




}
)

NODE_TYPE (Deg, "^", 9, {



}
)

NODE_TYPE (Var, "x", 10, {




}
)

NODE_TYPE (Num, "0", 11, {

    SET (OUT, "0", type_Num);

}
)




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


