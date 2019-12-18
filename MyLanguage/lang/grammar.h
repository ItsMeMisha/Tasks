#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include "Tree_t.h"
#include "tokens.h"
#include <assert.h>

Tree_node** CurTok = nullptr;

Tree* GetTree (const char* fileName, Tree* tree);

Tree_node* GetG    (Tree_node** Tokens);
Tree_node* GetD    ();
Tree_node* GetFunc ();
Tree_node* GetFId  ();
Tree_node* GetVarlist ();
Tree_node* GetDef  ();
Tree_node* GetOp   ();
Tree_node* GetRead ();
Tree_node* GetPrnt ();
Tree_node* GetDS   ();
Tree_node* GetVar  ();
Tone_t     GetTone ();
Tree_node* GetVId  ();
Tree_node* GetInit ();
Tree_node* GetWhile();
Tree_node* GetIf   ();
Tree_node* GetCond ();
Tree_node* GetComp ();
Tree_node* GetE    ();
Tree_node* GetT    ();
Tree_node* GetF    ();
Tree_node* GetP    ();
Tree_node* GetN    ();
Tree_node* GetId   ();
Tree_node* GetArglist ();

Tree* GetTree (const char* fileName, Tree* tree) {

    struct stat fileInfo = {};
    stat (fileName, &fileInfo);
    char* Content = (char*) calloc (fileInfo.st_size + 1, sizeof (char));
    FILE* file = fopen (fileName, "r");

    if (fread (Content, sizeof (char), fileInfo.st_size, file) == 0)
        printf ("Ah shit!\n");;

    Content[fileInfo.st_size -1] = EOF;

    Tree_node** Tokens = Tokenization (Content);
    free (Content);

    int i = 0;

    tree -> root = GetG (Tokens);
    free (Tokens);

    return tree;

}

Tree_node* GetG (Tree_node** Tokens) {

    CurTok = Tokens;

    Tree_node* val = GetD ();

    Tree_node* TreeBeg = val;

    while (*CurTok != nullptr) {

        val -> left = GetD ();
        val = val -> left;

    }

    return TreeBeg;

}

Tree_node* GetD () {

    assert (*CurTok);

    if ((*CurTok) -> type != TDec)
        return nullptr;

    Tree_node* val = NewNode (D_dec, TDec);

    if ((*CurTok) -> data.Ddata == D_leg) {

        if ((val -> right = GetFunc ()) == nullptr)
            return nullptr;

    }
 
    else if ((*CurTok) -> data.Ddata == D_nota) {

        if ((val -> right = GetVar ()) == nullptr)
            return nullptr;

    }

    else return nullptr;

    return val;

}

Tree_node* GetFunc () {

    assert (*CurTok);

    if ((*CurTok) -> type != TDec || (*CurTok) -> data.Ddata != D_leg)
        return nullptr;

    Tree_node* val = *CurTok;
    CurTok++;

    LinkNodes (val, nullptr, GetFId ());

    if (val -> right == nullptr)
        return nullptr;

    LinkNodes (val, GetVarlist (), nullptr);

    if ((*CurTok) -> data.Odata != OP_dbar || (*CurTok) -> type != TOp)
        return nullptr;

    CurTok++;

    LinkNodes (val -> right, nullptr, GetDef ());

    if (val -> right -> right == nullptr)
        return nullptr;

    return val;

}

Tree_node* GetFId () {

    assert (*CurTok);

    if ((*CurTok) -> type != TId)
        return nullptr;

    CurTok++;

    return *(CurTok - 1);

}

Tree_node* GetVarlist () {

    assert (*CurTok);

    Tree_node* val = *CurTok;

    if (val -> type != TOp || val -> data.Odata != OP_opbr)
        return nullptr;

    Tree_node* listbegin = val;

    listbegin -> data.Odata = OP_com;

    CurTok++;

    if ((*CurTok) -> type  == TOp && (*CurTok) -> data.Odata == OP_clbr) { 

        CurTok++;
        return nullptr;

    }
 
    while ((*CurTok) -> type == TId) {

        LinkNodes (val, nullptr, *CurTok);

        CurTok++;

        if (((*CurTok -1) -> tone = GetTone ()) == 0)
            return nullptr;

        if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_com) {

            LinkNodes (val, *CurTok, nullptr);
            val = val -> left;
            CurTok++;

        }

          
    }

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_clbr)
        return nullptr;

    CurTok++;

    return listbegin;

}

Tree_node* GetDef () {

    assert (*CurTok);

    Tree_node* val = NewNode (D_op, TDec);
    Tree_node* defBeg = val;


    while ((((*CurTok) -> data.Odata != OP_clwhl && (*CurTok) -> data.Odata != OP_dbar) && ((*CurTok) -> type == TOp)) || (*CurTok) -> type == TDec || (*CurTok) -> type == TId) {

        if (((*CurTok) -> type == TDec) && ((*CurTok) -> data.Ddata == D_nota)) {

            val -> data.Ddata = D_op;
            LinkNodes (val, nullptr, GetVar ());

            if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_bar)
                return nullptr;

            CurTok++;
 
        }

        else LinkNodes (val, nullptr, GetOp ());

        if (val -> right == nullptr)
            return nullptr;
       
        LinkNodes (val, NewNode (D_op, TDec), nullptr);

        val = val -> left;

    }

    if (val -> parent != nullptr) {

        val -> parent -> left = nullptr;
        free (val);

    }

    if ((*CurTok) -> type != TOp || ((*CurTok) -> data.Odata != OP_clwhl && (*CurTok) -> data.Odata != OP_dbar))
        return nullptr;
    CurTok++; 

    return defBeg;

}

Tree_node* GetOp () {

    Tree_node* val = nullptr;

    if ((val = GetId ()) != nullptr) {

        if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_opbr) {

            LinkNodes (val, GetArglist (), nullptr);

            if (val -> left == nullptr)
                return nullptr;

            if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_bar)
                return nullptr;

            CurTok++;

        } else {

            assert (*CurTok);

            if ((*CurTok) -> type != TOp && (*CurTok) -> data.Odata != OP_ass)
                return nullptr;
        
            (*CurTok) -> left = val;
            val = *CurTok;
            val -> right = GetInit (); 
    
            if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_bar)
                return nullptr;

            CurTok++;

        }
        
    }

    else if ((val = GetWhile ()) != nullptr);

    else if ((val = GetIf ()) != nullptr);

    else if ((val = GetDS ()) != nullptr);

    else if ((val = GetRead ()) != nullptr);

    else if ((val = GetPrnt ()) == nullptr)
        return nullptr; 

    return val;

}

Tree_node* GetRead () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_read)
        return nullptr;

    Tree_node* val = *CurTok;
    CurTok++;

    LinkNodes (val, nullptr, GetId());

    if (val -> right == nullptr)
        return nullptr;

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_bar) {

        CurTok++;
        return val;

    }        

    return val;

}

Tree_node* GetPrnt () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_prnt)
        return nullptr;

    Tree_node* val = *CurTok;
    CurTok++;

    LinkNodes (val, nullptr, GetE());

    if (val -> right == nullptr)
        return nullptr;

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_bar) {

        CurTok++;
        return val;

    }        

    return val;

}

Tree_node* GetArglist () {

    assert (*CurTok);

    Tree_node* val = *CurTok;

    if (val -> type != TOp || val -> data.Odata != OP_opbr)
        return nullptr;

    Tree_node* listbegin = val;

    listbegin -> data.Odata = OP_com;

    CurTok++;

    if ((*CurTok) -> type  == TOp && (*CurTok) -> data.Odata == OP_clbr) { 

        CurTok++;
        return nullptr;

    }

    while ((val -> right = GetE ()) != nullptr) {

        if ((*CurTok) -> type == TOp && ((*CurTok) -> data.Odata == OP_com) || (*CurTok) -> data.Odata == OP_clbr) {

            LinkNodes (val, *CurTok, nullptr);
            val = val -> left;

            if ((*CurTok) -> data.Odata == OP_clbr)
                break;

            else CurTok++;

        }

        else break;
            
    }

    if (val -> parent != nullptr) {

        val = val -> parent;
        val -> left = nullptr;

    }

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_clbr)
        return nullptr;

    CurTok++;

    return listbegin;

}

Tree_node* GetDS () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_ds)
        return nullptr;

    Tree_node* val = *CurTok;
    CurTok++;

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_bar) {

        CurTok++;
        return val;

    }        

    LinkNodes (val, nullptr, GetE());

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_bar) {

        CurTok++;
        return val;

    }        

    return val;

}

Tree_node* GetVar () {

    assert (*CurTok);

    if ((*CurTok) -> type != TDec || (*CurTok) -> data.Ddata != D_nota)
        return nullptr;

    Tree_node* val = *CurTok;
    CurTok++;

    LinkNodes (val, nullptr, GetVId ());
    
    if (val -> right == nullptr)
        return nullptr;

    if ((val -> tone = GetTone ()) == 0)
        return nullptr;

    LinkNodes (val, GetInit (), nullptr);

    if (val -> left == nullptr)
        return nullptr;

    return val;

}

Tone_t GetTone () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_ass)
        return 0;

    CurTok++;

    if ((*CurTok) -> type != TNote)
        return 0;

    CurTok++;

    return (*(CurTok - 1)) -> tone;

}

Tree_node* GetVId () {

    assert (*CurTok);

    if ((*CurTok) -> type != TId)
        return nullptr;

    CurTok++;

    return *(CurTok - 1);

}

Tree_node* GetInit () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_ass)
        return nullptr;

    CurTok++;
    
    return GetE ();

}

Tree_node* GetWhile () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_whl)
        return nullptr;

    Tree_node* val = *CurTok;

    CurTok++;

    LinkNodes (val, GetCond (), nullptr);

    LinkNodes (val, nullptr, GetDef ());

    if ((*(CurTok - 1)) -> type != TOp || (*(CurTok - 1)) -> data.Odata != OP_clwhl)
        return nullptr;

    return val;

}

Tree_node* GetIf () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_vol)
        return nullptr;
    
    Tree_node* val = *CurTok;
    CurTok++;

    LinkNodes (val, GetCond (), nullptr);

    LinkNodes (val, nullptr, NewNode (OP_vol, TCon));

    if ((*CurTok) -> data.Odata != OP_dbar || (*CurTok) -> type != TOp)
        return nullptr;

    CurTok++;

    LinkNodes (val -> right, nullptr, GetDef ());

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_svol) {

        CurTok++;
        if ((*CurTok) -> data.Odata != OP_dbar || (*CurTok) -> type != TOp)
            return nullptr;

            CurTok++;

        LinkNodes (val -> right, GetDef (), nullptr);

    }

    return val;

}

Tree_node* GetCond () {

    assert (*CurTok);
    
    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_opbr)
        return nullptr;

    CurTok++;

    Tree_node* val1 = GetE ();
    Tree_node* val = GetComp ();
    Tree_node* val2 = GetE ();

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_clbr)
        return nullptr;

    CurTok++;

    LinkNodes (val, val1, val2);

    return val;

}

Tree_node* GetComp () {

    assert (*CurTok);

    if ((*CurTok) -> type == TOp) {

        OpType comp = (*CurTok) -> data.Odata;

        if (comp == OP_meno || comp == OP_piu || comp == OP_eq || comp == OP_neq) {

            CurTok++;
            return *(CurTok - 1);

        }

    }

    return nullptr;

}

Tree_node* GetE () {

    assert (*CurTok);

    Tree_node* val1 = GetT ();

    Tree_node* val = val1;

    if (val == nullptr) {

        if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_sub) {

            CurTok++;
            val1 = GetT ();
            val = NewNode (OP_mul, TOp);
            val -> left = NewNode ((double)(-1), TNum);
            val -> right = val1;
           
        } else return nullptr;

    }

    assert (*CurTok);

    while (((*CurTok) -> type == TOp) && ((*CurTok) -> data.Odata == OP_add || (*CurTok) -> data.Odata == OP_sub)) {

        val = *CurTok;
        CurTok++;

        Tree_node* val2 = GetT ();

        LinkNodes (val, val1, val2);

        val1 = val;
        assert (*CurTok);

    }

    return val;

}

Tree_node* GetT () {

    Tree_node* val1 = GetF ();
    Tree_node* val = val1;

    assert (*CurTok);
 
    while (((*CurTok) -> type == TOp) && ((*CurTok) -> data.Odata == OP_mul || (*CurTok) -> data.Odata == OP_div)) {

        val = *CurTok;
        CurTok++;

        Tree_node* val2 = GetP ();

        LinkNodes (val, val1, val2);

        val1 = val;
        assert (*CurTok);

    }

    return val;

}

Tree_node* GetF () {

    assert (*CurTok);

    if ((*CurTok) -> type == TOp) {

        OpType Odat = (*CurTok) -> data.Odata;

        if (Odat == OP_cos || Odat == OP_sin || Odat == OP_sqrt) {

            Tree_node* val = *CurTok;
            CurTok++;
            LinkNodes (val, nullptr, GetP ());

            return val;

        }

        else return GetP ();

    }

    return GetP ();

}

Tree_node* GetP () {

    assert (*CurTok);

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_opbr) {

        CurTok++;
        Tree_node* val = GetE ();
        assert (*CurTok);

        if ((*CurTok) -> data.Odata != OP_clbr) {

            printf ("Error: unexpected token '%s', expected ')' \n",op[(*CurTok) -> data.Odata]);  
            return nullptr;

        }

        CurTok++;
        return val;

    }

    if ((*CurTok) -> type == TNum)
        return GetN ();

    return GetId ();

}

Tree_node* GetN () {
    
    assert (*CurTok);

    if ((*CurTok) -> type != TNum)
        return nullptr;

    CurTok++;

    return *(CurTok - 1);

}

Tree_node* GetId () {

    assert (*CurTok);

    if ((*CurTok) -> type != TId)
        return nullptr;

    CurTok++;

    return *(CurTok - 1);

}


