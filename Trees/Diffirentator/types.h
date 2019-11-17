
#define DIF(In, Out) NodeDiffer (In, Out, OutTree)

#define SET(OutNode, data, type) sscanf (data, "%ms", &(OutNode -> data)); OutNode -> type = type;

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

void CopyBranch (Tree_node* SourceBranch, Tree_node* OutBranch, Tree* OutTree) {

    ASSERTTREE (OutTree);
    assert (SourceBranch);
    assert (OutBranch);

    sscanf (SourceBranch -> data, "%ms", &(OutBranch -> data));
    OutBranch -> type = SourceBranch -> type;

    if (SourceBranch -> left != nullptr) {
        NEWL (OutBranch);
        CPY (SourceBranch -> left, OutBranch -> left);
    }

    if (SourceBranch -> right != nullptr) {
        NEWR (OutBranch);
        CPY (SourceBranch -> right, OutBranch -> right);
    }

    return;
    
}

//NODE_TYPE (type_name, type_signature, type_num, code)

NODE_TYPE (NoType, "", 0, return;  )

NODE_TYPE (Add, "+", 1, {

    CPYINOUT;
    NEWL;
    NEWR;
    DIF (INL, OUTL);
    DIF (INR, OUTR);

}
)

NODE_TYPE (Sub, "-", 2, {

    CPYINOUT;
    NEWL;
    NEWR;
    DIF (INL, OUTL)
    DIF (INR, OUTR)

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

    CPY (INL, OUTR -> left);
    SET (OUTR -> right, "2", type_Num);

    SET (OUTL -> left, "*", type_Mul);
    SET (OUTL -> right, "*", type_Mul);

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

NODE_TYPE (Num, "", 11, {





}
)




