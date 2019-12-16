#include <stdio.h>
#include "Tree_t.h"
#include "enum_type.h"
#include "std_op.h"
#include "funcs_and_vars.h"

void PrAsmCode ();
void PrAsmDec  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmFunc (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmDef  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
int  CountWriteArgs (AvVars* vars, Tree_node* Vlist); 
void PrAsmVar  (Vars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmVar  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmExp  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmWhl  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmIf   (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmAs   (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmCall (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmPushVlist (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);
void PrAsmCond (AvVars* vars, Tree_node* Cur, AllFuncs* funcs);


void PrAsmCode () {

    fprintf (file, "call :MainTheme\n end");

    var_t* vars = (var_t*) calloc (var_t, MaxVars);
    function* funcs = (functions*) calloc (function, MaxFunctions);

//TODO

    Tree_node* Cur = /*TODO*/ReadTree ();

    while (Cur != nullptr) {

        PrAsmDec (vars, Cur, funcs);
        Cur = Cur -> left;

    }

    return;

}

void PrAsmDec (Vars* vars, Tree_node* Cur, AllFunc*s funcs) {

    assert (Cur);

    assert (Cur -> left);

    if (Cur -> right -> type == TDec) {

        if (Cur -> right -> data,Ddata == D_leg)
            PrAsmFunc (vars, Cur -> right, funcs); 

        else (Cur -> right -> data.Ddata == D_nota)
            PrAsmVar (vars -> gVars, Cur -> right, funcs);

    }

    return;

}

void PrAsmFunc (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    assert (Cur);

    if (Cur -> right != nullptr && Cur -> right -> type == TId);
        fprintf (file, ":%s", Cur -> right -> Sdata);

    int argssNum = 0;

    AvVars* funcVars = ConstructAvVars (nullptr, vars -> shift + vars -> fVars -> size, vars -> gVars); 

    if (Cur -> left != nullptr)
        argsNum = CountWriteArgs (Cur -> left); 

    fprintf (file, "pop fx\npush %d\npop sx\n", funcVars -> shift);

    for (int i = 0; i < varsNum; i++)
        fprintf (file, "pop [sx + %d]\n", argsNum - i - 1);

    PrAsmDef (funcVars, Cur -> right -> right);

    return;

}

int CountWriteArgs (AvVars* vars, Tree_node* Vlist) {

    assert (vars);

    int ArgsNum = 1;

    while (Vlist -> left != nullptr) {

        assert (Vlist -> right);
        int pos = 0;

        ArgsNum++;
        Vlist = Vlist -> left;
        if (!AddVar (vars -> fVars, Vlist -> right -> data.Sdata, pos);
            return 0;

        pos++;

    }

    return ArgsNum; 

}

void PrAsmDef (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    

    return;

}

void PrAsmVar (Vars* vars, Tree_node* Cur, AllFuncs* funcs) {

    assert (vars);

    if (Cur -> type != TDec || Cur -> data.Ddata != D_nota)
        return;

    assert (Cur -> left);

    if (Cur -> left -> type != TId)
        return;

    int pos = -1;

    if ((pos = FindVar (vars, Cur -> left -> data.Sdata)) >= 0)
        return;

    AddVar (vars, Cur -> left -> data.Sdata, pos);

    if (Cur -> right == nullptr)
        return;

    AvVars* globalVars = ConstructAvVars (nullptr, 0, vars -> gVars);  

    PrAsmExp (globalVars, Cur -> right, funcs);
    fprintf (file, "pop [%d]\n", pos);

    return;

}

void PrAsmVar (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    assert (vars);

    if (Cur -> type != TDec || Cur -> data.Ddata != D_nota)
        return;

    assert (Cur -> left);

    if (Cur -> right == nullptr)
        return; 

    PrAsmExp (vars, Cur -> right, funcs);

    if (Cur -> left -> type != TId)
        return;

    int pos = -1;

    if ((pos = FindVar (vars -> fVars, Cur -> left -> data.Sdata)) >= 0)
        return;

    AddVar (vars -> fVars, Cur -> left -> data.Sdata, pos);

    fprintf (file, "pop [%d]\n", vars -> shift + pos);

    return;

}

void PrAsmExp (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    if (Cur == nullptr)
        return;

    if (Cur -> type == TNum) {

        fprintf (file, "push %lg\n", Cur -> data.Ndata);
        return;

    }

    if (Cur -> type == TOp) {
    
        if (Cur -> left != nullptr)
            PrAsmExp (vars, Cur -> left);

        if (Cur -> right != nullptr)
            PrAsmExp (vars, Cur -> right);

        switch (Cur -> data.Odata) {

        case OP_add: fprintf (file, "add\n"); break;

        case OP_sub: fprintf (file, "sub\n"); break;

        case OP_mul: fprintf (file, "mul\n"); break;

        case OP_div: fprintf (file, "div\n"); break;

        case OP_sin: fprintf (file, "sin\n"); break;

        case OP_cos: fprintf (file, "cos\n"); break; 

        case OP_mod: /*TODO*/;

        }

        return;

    }

    if (Cur -> type == TVar) {

        int position = 0;

        fprintf (file, "push [");

        if ((position = FindVar (vars -> gVars, Cur -> data.Sdata)) < 0) {

            if ((position = FindVar (vars -> fVars, Cur -> data.Sdata)) < 0)
                return;
    
            else fprintf (file, "%d]\n", position + vars -> shift);

        }

        else fprintf (file, "%d]\n", position);

    }

    if (Cur -> type == TFunc) {

        PrAsmCall (vars, Cur, funcs);

    }

    return;

}

void PrAsmWhl (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    assert (Cur);

    fprintf (file, ":while%d\n", /*TODO while num*/);

    if (Cur -> type != TOp || Cur -> data.Odata != OP_whl)
        return;

    if (Cur -> right != nullptr)
        PrAsmDef (vars, Cur -> right, funcs);

    PrAsmCond (vars, Cur -> left, funcs);
    fpritnt (file, " :shile%d\n", /*TODO while num*/);

    return;

}

void PrAsmIf (AvVars vars*, Tree_node* Cur, AllFuncs* funcs) {

    assert (Cur);

    if (Cur -> type != TOp || Cur -> data.Odata != OP_vol)
        return;

    if (Cur -> left == nullptr || Cur -> left -> type != TOp)
        return;

    if (Cur -> left -> left == nullptr || Cur -> left -> right == nullptr)
        return;

    PrMExp (vars, Cur -> left -> left);
    PrMExp (vars, Cur -> left -> rigth);

    switch (Cur -> left -> data.Odata) {

        case OP_meno: fprintf (file, "ja :"); break;

        case OP_piu: fprintf (file, "jb :"); break;

        case OP_eq: fprintf (file, "je :"); break;

        case OP_neq: fprintf (file, "jne :"); break;

        default: fprintf (file, "pop\npop\n"); return;

    }

    fprintf (file, "else%d\n", /*TODO if num*/);

    if (Cur -> right == nullptr)
        return;

    PrAsmDef (Cur -> right -> right);
    fprintf (file, "jmp :endif%d\n:else%d", /*TODO*/);
    PrAsmDef (Cur -> right -> left);
    fprintf (file, ":endif%d\n", /*TODO*/);
    return;

}

void PrAsmAs (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    assert (Cur);

    if (Cur -> type != TOp || Cur -> data.OData != OP_ass)
        return;

    if (Cur -> left == nullptr || Cur -> right == nullptr)
        return;

    if (Cur -> left -> type != TVar)
        return;

    PrMExp (vars, Cur -> right);

    int pos = 0;
    
    if ((pos = FindVar (vars -> gVars)) < 0) {

        if ((pos = FindVar (vars -> fVars)) < 0)
            return;

        else fprintf (file, "pop [%d]\n", pos + vars -> shift);

    } else fprintf (file, "pop [%d]\n", pos);

    return;

}

void PrAsmCall (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    assert (Cur);

    fprintf (file, "push Fx\n");

    if (Cur -> left != nullptr)
        PrAsmPushVlist (vars, Cur -> left);

    printf (file, "call :%s\n", Cur -> Sdata);

    return;

}

void PrAsmPushVlist (AvVars vars, Tree_node* Cur, AllFuncs funcs) {

    while (Cur != nullptr && Cur -> left != nullptr) {

        if (Cur -> left -> type == TOp)
            PrAsmExp (vars, Cur, funcs);

        else if (Cur -> left -> type == TNum)
            fprintf (file, "push %lg\n", Cur -> left -> data.Ndata);

        else if (Cur -> left -> type == TId) {
     
            int pos = 0;

            if ((pos = FindVar (vars -> gVars, Cur -> data.Sdata)) < 0) {

                if ((pos = FindVar (vars -> fVars, Cur -> data.Sdata)) < 0) {

                    if ((pos = FindFunc (funcs, Cur -> data.Sdata)) < 0)
                        return;

                    else PrAsmCall (vars, Cur, funcs);
                    
            } else fprintf (file, "push [%d]\n", pos + vars -> shift);

            }

            else fprintf (file, "push [%d]\n", pos);

        } else return;

        Cur = Cur -> right;   

    }

    return;

}

void PrAsmCond (AvVars* vars, Tree_node* Cur, AllFuncs* funcs) {

    return;

}

