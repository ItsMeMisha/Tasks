#ifndef __BACKEND__

    #define __BACKEND__

#include <stdio.h>
#include "Tree_t.h"
#include "enum_type.h"
#include "std_op.h"
#include "funcs_and_vars.h"

struct Support {

    int WhileNum;
    int IfNum;
    FILE* file;

};

void PrAsmCode (Tree* tree, FILE* file);
void PrAsmDec  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmFunc (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmDef  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
int  CountWriteArgs (AvVars* vars, Tree_node* Vlist);
void PrAsmRet  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf); 
void PrAsmVar  (Vars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmVar  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmExp  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmWhl  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmIf   (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmAs   (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmCall (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmPushVlist (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmCond (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmWhlCond (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void OposCond (Tree_node* node); 
void PrAsmRead (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);
void PrAsmPrnt (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf);


void PrAsmCode (Tree* tree, FILE* file) {

    assert (tree);
    assert (file);

    fprintf (file, "push 0\npop sx\ncall :MainTheme\njmp :progend\n");

    AvVars* vars = ConstructAvVars (nullptr, 0, nullptr);
    AllFuncs* funcs = ConstructFuncs (nullptr);
    Support SupInf = {0, 0, file};

    Tree_node* Cur = tree -> root;

    while (Cur != nullptr) {

        PrAsmDec (vars, Cur, funcs, &SupInf);
        Cur = Cur -> left;

    }

    DestructVars (vars -> gVars);
    DestructAvVars (vars);
    DestructFuncs (funcs);

    fprintf (file, "\n:progend\nend\n");

    return;

}

void PrAsmDec (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    assert (Cur -> right);

    if (Cur -> right -> type == TDec) {

        if (Cur -> right -> data.Ddata == D_leg)
            PrAsmFunc (vars, Cur -> right, funcs, SupInf); 

        else if (Cur -> right -> data.Ddata == D_nota)
            PrAsmVar (vars -> gVars, Cur -> right, funcs, SupInf);

    }

    return;

}

void PrAsmFunc (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    FILE* file = SupInf -> file;

    if (Cur -> right != nullptr && Cur -> right -> type == TId);
        fprintf (file, "\n:%s\n", Cur -> right -> data.Sdata);
    fprintf (file, "pop Fx\n");

    int argsNum = 0;

    AvVars* funcVars = ConstructAvVars (nullptr, vars -> shift + vars -> fVars -> size, vars -> gVars); 

    if (Cur -> left != nullptr)
        argsNum = CountWriteArgs (funcVars, Cur -> left);

    for (int i = 0; i < argsNum; i++)
        fprintf (file, "pop [sx + %d]\n", argsNum - i - 1);

    AddFunc (funcs, Cur -> right -> data.Sdata, argsNum);

    PrAsmDef (funcVars, Cur -> right -> right, funcs, SupInf);

    DestructVars (funcVars -> fVars);
    free (funcVars);

    return;

}

int CountWriteArgs (AvVars* vars, Tree_node* Vlist) {

    assert (vars);

    int pos = 0;

    while (Vlist != nullptr) {

        assert (Vlist -> right);

        if (!AddVar (vars -> fVars, Vlist -> right -> data.Sdata, pos))
            return 0;
        Vlist = Vlist -> left;
        pos++;

    }

    return pos; 

}

void PrAsmDef (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    FILE* file = SupInf -> file;

    while (Cur != nullptr && Cur -> right != nullptr) {

        if (Cur -> right -> type == TDec && Cur -> data.Ddata == D_nota)
            PrAsmVar (vars, Cur, funcs, SupInf);

        else if (Cur -> right -> type == TOp) {

            switch (Cur -> right -> data.Odata) {

                case OP_vol: PrAsmIf (vars, Cur -> right, funcs, SupInf); break;

                case OP_whl: PrAsmWhl (vars, Cur -> right, funcs, SupInf); break;

                case OP_ass: PrAsmAs (vars, Cur -> right, funcs, SupInf); break;

                case OP_ds: PrAsmRet (vars, Cur -> right, funcs, SupInf); break;

                case OP_read: PrAsmRead (vars, Cur -> right, funcs, SupInf); break;
    
                case OP_prnt: PrAsmPrnt (vars, Cur -> right, funcs, SupInf); break;

                case OP_call: PrAsmCall (vars, Cur -> right, funcs, SupInf);

                    fprintf (file, "pop ax\n");
                    
                    break;

                default: return;

            }

        }
 
        else if (Cur -> right -> type == TDec) {

            PrAsmVar (vars, Cur -> right, funcs, SupInf);

        } else return;

        Cur = Cur -> left;
      
    }

    return;

}

void PrAsmRet  (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    FILE* file = SupInf -> file;

    if (Cur -> right == nullptr)
        fprintf (file, "push 0\n");
    
    else PrAsmExp (vars, Cur -> right, funcs, SupInf);

    fprintf (file, "push Fx\nret\n");

    return;

}

void PrAsmVar (Vars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (vars);

    FILE* file = SupInf -> file;

    if (Cur -> type != TDec || Cur -> data.Ddata != D_nota)
        return;

    assert (Cur -> right);

    if (Cur -> right -> type != TId)
        return;

    int pos = -1;

    if ((pos = FindVar (vars, Cur -> right -> data.Sdata)) >= 0)
        return;

    pos = vars -> size;

    AddVar (vars, Cur -> right -> data.Sdata, pos);

    if (Cur -> right == nullptr)
        return;

    AvVars* globalVars = ConstructAvVars (nullptr, 0, vars);  

    PrAsmExp (globalVars, Cur -> left, funcs, SupInf);
    fprintf (file, "pop [%d]\n", pos);

    return;

}

void PrAsmVar (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (vars);
    
    FILE* file = SupInf -> file;

    if (Cur -> type != TDec || Cur -> data.Ddata != D_nota)
        return;

    if (Cur -> right == nullptr)
        return; 

    PrAsmExp (vars, Cur -> left, funcs, SupInf);

    if (Cur -> right -> type != TId)
        return;

    int pos = -1;

    if ((pos = FindVar (vars -> fVars, Cur -> right -> data.Sdata)) >= 0)
        return;

    pos = vars -> fVars -> size;

    AddVar (vars -> fVars, Cur -> right -> data.Sdata, pos);

    fprintf (file, "pop [sx + %d]\n", pos);

    return;

}

void PrAsmExp (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    if (Cur == nullptr)
        return;

    FILE* file = SupInf -> file;

    if (Cur -> type == TNum) {

        fprintf (file, "push %lg\n", Cur -> data.Ndata);
        return;

    }

    if (Cur -> type == TOp) {

        if (Cur -> data.Odata == OP_com)
            return;
    
        if (Cur -> left != nullptr)
            PrAsmExp (vars, Cur -> left, funcs, SupInf);

        if (Cur -> right != nullptr) 
            PrAsmExp (vars, Cur -> right, funcs, SupInf);

        switch (Cur -> data.Odata) {

        case OP_add: fprintf (file, "add\n"); break;

        case OP_sub: fprintf (file, "sub\n"); break;

        case OP_mul: fprintf (file, "mul\n"); break;

        case OP_div: fprintf (file, "div\n"); break;

        case OP_sin: fprintf (file, "sin\n"); break;

        case OP_cos: fprintf (file, "cos\n"); break; 

        case OP_sqrt: fprintf (file, "sqrrt\n"); break;

        case OP_call: PrAsmCall (vars, Cur, funcs, SupInf); break;

        default: return; 

        }

        return;

    }

    if (Cur -> type == TId) {

        int position = -1;

        if ((position = FindVar (vars -> fVars, Cur -> data.Sdata)) < 0) {

            if ((position = FindVar (vars -> gVars, Cur -> data.Sdata)) < 0)
                return;
    
            else fprintf (file, "push [%d]\n", position);

        }

        else fprintf (file, "push [sx + %d]\n", position);

    }

    return;

}

void PrAsmWhl (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    FILE* file = SupInf -> file;

    SupInf -> WhileNum++;
    int whileNum = SupInf -> WhileNum;

    if (Cur -> type != TOp || Cur -> data.Odata != OP_whl)
        return;

    PrAsmCond (vars, Cur -> left, funcs, SupInf);
    fprintf (file, ":while%d\n", whileNum);

    if (Cur -> right != nullptr)
        PrAsmDef (vars, Cur -> right, funcs, SupInf);

    PrAsmWhlCond (vars, Cur -> left, funcs, SupInf);
    fprintf (file, " :while%d\n:endwhile%d\n", whileNum, whileNum);

    return;

}

void PrAsmIf (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    FILE* file = SupInf -> file;

    if (Cur -> type != TOp || Cur -> data.Odata != OP_vol)
        return;

    if (Cur -> left == nullptr || Cur -> left -> type != TOp)
        return;

    if (Cur -> left -> left == nullptr || Cur -> left -> right == nullptr)
        return;

    PrAsmCond (vars, Cur -> left, funcs, SupInf);

    SupInf -> IfNum++;
    int ifNum = SupInf -> IfNum;

    fprintf (file, ":else%d\n", ifNum);

    if (Cur -> right == nullptr)
        return;

    PrAsmDef (vars, Cur -> right -> right, funcs, SupInf);
    fprintf (file, "jmp :endif%d\n:else%d\n", ifNum, ifNum);
    PrAsmDef (vars, Cur -> right -> left, funcs, SupInf);
    fprintf (file, ":endif%d\n", ifNum);
    return;

}

void PrAsmAs (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    FILE* file = SupInf -> file;

    if (Cur -> type != TOp || Cur -> data.Odata != OP_ass)
        return;

    if (Cur -> left == nullptr || Cur -> right == nullptr)
        return;

    if (Cur -> left -> type != TId)
        return;

    PrAsmExp (vars, Cur -> right, funcs, SupInf);

    int pos = -1;

    if ((pos = FindVar (vars -> fVars, Cur -> left -> data.Sdata)) < 0) {

        if ((pos = FindVar (vars -> gVars, Cur -> left -> data.Sdata)) < 0)
            return;

        else fprintf (file, "pop [%d]\n", pos);

    } else fprintf (file, "pop [sx+%d]\n", pos);

    return;

}

void PrAsmCall (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    if (Cur -> type != TOp || Cur -> data.Odata != OP_call)
        return;

    if (FindFunc (funcs, Cur -> right -> data.Sdata) < 0)
        return;

    FILE* file = SupInf -> file;

    fprintf (file, "push Fx\n");

    if (Cur -> left != nullptr)
        PrAsmPushVlist (vars, Cur -> left, funcs, SupInf);

    fprintf (file, "push %d\npush sx\nadd\npop sx\n", vars -> fVars -> size);
    fprintf (file, "call :%s\n", Cur -> right -> data.Sdata);
    fprintf (file, "pop ax\npop Fx\npush ax\n");
    fprintf (file, "push sx\npush %d\nsub\npop sx\n", vars -> fVars -> size);  

    return;

}

void PrAsmPushVlist (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    FILE* file = SupInf -> file;

    while (Cur != nullptr && Cur -> right != nullptr) {

        PrAsmExp (vars, Cur -> right, funcs, SupInf);

        Cur = Cur -> left;   

    }

    return;

}

void PrAsmCond (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);
    assert (SupInf);

    FILE* file = SupInf -> file;

    if (Cur == nullptr || Cur -> left == nullptr || Cur -> right == nullptr)
        return;

    PrAsmExp (vars, Cur -> left, funcs, SupInf);
    PrAsmExp (vars, Cur -> right, funcs, SupInf);

    if (Cur -> type != TOp)
        return;

    switch (Cur -> data.Odata) {

        case OP_meno: fprintf (file, "jbe "); break;

        case OP_piu: fprintf (file, "jae "); break;

        case OP_eq: fprintf (file, "jne "); break;

        case OP_neq: fprintf (file, "je "); break;

        default: fprintf (file, "pop\npop\n"); return;

    }

    return;

}

void PrAsmWhlCond (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    OposCond (Cur);

    PrAsmCond (vars, Cur, funcs, SupInf);

    OposCond (Cur);

    return;

}

void OposCond (Tree_node* node) {

    assert (node);

    if (node -> type != TOp)
        return;

    switch (node -> data.Odata) {

        case OP_meno: node -> data.Odata = OP_piu; break;

        case OP_piu: node -> data.Odata = OP_meno; break;

        case OP_eq: node -> data.Odata = OP_neq; break;

        case OP_neq: node -> data.Odata = OP_eq; break;

    }

    return;
}

void PrAsmRead (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    FILE* file = SupInf -> file;

    fprintf (file, "in\n");

    int pos = -1;

    if (Cur -> right == nullptr)
        return;
    
    else if ((pos = FindVar (vars -> fVars, Cur -> right -> data.Sdata)) < 0) {

        if ((pos = FindVar (vars -> gVars, Cur -> right -> data.Sdata)) < 0)
            return;

        else fprintf (file, "pop [%d]\n", pos);

    } else fprintf (file, "pop [sx+%d]\n", pos);

    return;

}

void PrAsmPrnt (AvVars* vars, Tree_node* Cur, AllFuncs* funcs, Support* SupInf) {

    assert (Cur);

    FILE* file = SupInf -> file;

    int pos = -1;

    if (Cur -> right == nullptr)
        return;

    else if (Cur -> right -> type != TId) 
             PrAsmExp (vars, Cur -> right, funcs, SupInf);

    else if ((pos = FindVar (vars -> fVars, Cur -> right -> data.Sdata)) < 0) {

        if ((pos = FindVar (vars -> gVars, Cur -> right -> data.Sdata)) < 0)
            return;
        else fprintf (file, "push [%d]\n", pos);

    } else fprintf (file, "push [sx + %d]\n", pos);

    fprintf (file, "out\n");

    return;

}
#endif
