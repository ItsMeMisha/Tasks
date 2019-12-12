#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include "Tree_t.h"

const char* CurChar = "";
const Tree_node** CurTok = nullptr;

void SkipSpace ();

Tree* GetTree (const char* fileName, Tree* tree);

Tree_node* GetG    (const char* str);
Tree_node* GetD    ();
Tree_node* GetFunc ();
Tree_node* GetFId  ();
Tree_node* GetVarlist ();
Tree_node* Def     ();
Tree_node* Op      ();
Tree_node* Var     ();
Tree_node* VId     ();
Tree_node* GetInit ();
Tree_node* GetWhile();
Tree_node* GetIf   ();
Tree_node* GetCond ();
Tree_node* GetComp ();
Tree_node* GetE    ();
Tree_node* GetT    ();
Tree_node* GetP    ();
Tree_node* GetN    ();
double     GetMt   ();
Tree_node* GetId   ();

bool isspec (char ch);
Tree_node** Tokenization (char* text);
Tree_node*  TokWord ();
Tree_node*  TokNum  ();
Tree_node*  TokSpec ();
Tree_node*  TokNote ();

void SkipSpace () {

    while (*CurChar == ' ' || *CurChar == '\n' || *CurChar == '\r' || *CurChar == '\t')
        CurChar++;

    return;

}

Tree* GetTree (const char* fileName, Tree* tree) {

    struct stat fileInfo = {};
    stat (fileName, &fileInfo);
    char* Content = (char*) calloc (fileInfo.st_size , sizeof (char));
    FILE* file = fopen (fileName, "r");

    fread (Content, sizeof (char), fileInfo.st_size, file);
    
    tree -> root = GetG (Content);

    free (Content);

    return tree;

}

Tree_node* GetG (const char* str) {

    SkipSpace ();

    CurChar = str;
        
    Tree_node* val = GetD ();

    while (val != nullptr) {

        val -> left = GetD ();
        val = val -> left;

    }

    if (*CurChar != '\0') {

        printf ("Error: invalid syntax near %.10s\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    return val;

}

Tree_node* GetD () {

    if (CurTok -> NodeType != TDec)
        return nullptr;

    Tree_node* val = CurTok;
    CurTok++;

    if (val -> data.Ddata == D_note) {

        if ((val -> right = GetFunc ()) == nullptr)
            return nullptr;

    }
 
    else if (val -> data.Ddata == D_leg) {

        if ((val -> right = GetVar ()) == nullptr)
            return nullptr;

    }

    else return nullptr;

    return val;

}

Tree_node* GetFunc () {

    Tree_node* val = NewNode (D_leg, TDec);

    if ((val -> left = GetVarlist ()) == nullptr)
        return nullptr;

    if ((val -> right = GetFId ()) == nullptr)
        return nullptr;

    if ((val -> right -> right = Def ()) == nullptr)
        return nullptr;

    return val;

}

Tree_node* GetFId () {

    if (CurTok -> NodeType != TId)
        return nullptr;

    CurTok++;

    return (CurTok - 1);

}

Tree_node* GetVarlist () {

    SkipSpace ();

    Tree_node* val = NewNode ();

    while

    return val;

}

Tree_node* Def () {

    if (!(CurTok -> data.Odata == OP_dbar && CurTok -> type == OpType))
        return nullptr;

    CurTok++;

    while ((!(CurTok -> data.Odata == OP_dbar && CurTok -> type == OpType)) /* TODO */) {

        
if ((CurTok -> type == TDec) && (CurTok -> data.Ddata == D_nota)) {

            val = NewNode ();
            val -> right = GetVar ();

        }

        else 

        if ((val -> right = GetOp ()) == nullptr);
    
        val -> left = NewNode ();
        val = val -> left;

    }

    return val;

}

Tree_node* Op () {

    Tree_node* val = nullptr;

    if ((val = VId ()) != nullptr) {

        if (CurTok -> type != TOp && CurTok -> data.Odata != OP_ass)
            return nullptr;

                 

    }

    else if ((val = GetFId ()) != nullptr) {

        if ((val -> left = GetVarlist ()) == nullptr)
            return nullptr;

    }

    else if ((val = GetWhile ()) == nullptr)
            return nullptr;

    else if ((val = GetIf ()) == nullptr)
        return nullptr; 

    return val;

}

Tree_node* Var () {

    Tree_node* val = NewNode (D_nota, TDec);
    
    if ((val -> right = GetVId ()) == nullptr)
        return nullptr;

    if ((val -> tone = GetTone ()) == nullptr)
        return nullptr;

    if ((val -> left = GetInit ()) == nullptr)
        return nullptr;

    return val;

}

Tree_node* VId () {

    retunr val;

}

Tree_node* GetInit () {

    if (*CurChar != '=')
        return nullptr;

    CurChar++;
    Tree_node* val = NewNode ();

    val -> right = GetE ();
    
    return val;

}

Tree_node* GetWhile () {

    SkipSpace ();

    if (strncmp (CurChar, "reprisa", 7) != 0)
        return nullptr;

    CurChar += 7;
    
    Tree_node* val = NewNode ();

    val -> left = GetCond ();
    val -> right = GetDef;

    return val;

}

Tree_node* GetIf () {

    SkipSpace ();

    if (CurTok -> type != TOp || CurTok -> data.Odata != OP_volta)
        return nullptr;
    
    Tree_node* val = NewNode ();

    CurTok++;

    val -> left = GetCond ();
    val -> right = NewNode ();
    val -> right -> right = GetDef;

    if (CurTok -> type == TOp && CurTok -> data.Odata == OP_svol) {

        CurTok++;
        val -> right -> left = GetDef;

    }

    return val;

}

Tree_node* GetCond () {

    SkipSpace ();

    Tree_node* val1 = GetE ();
    Tree_node* val = GetComp ();
    Tree_node val2 = GetE ();

    val -> left = val1;
    val -> right = val2;

    return val;

}

Tree_node* GetComp () {

    assert (CurTok);

    if (CurTok -> type == TOp) {

        OpType comp = CurTok -> data.Odata; 

        if (comp == OP_meno || comp == OP_piu || comp == OP_eq || comp == OP_neq)
            return CurTok;

    }

    return nullptr;

}

Tree_node* GetE () {

    assert (CurTok)

    if (CurTok -> type != TOp && CurTok -> type != TNum)
        return nullptr;

    Tree_node* val1 = GetT ();

    Tree_node* val = val1;

    

    while (*CurChar == '+' || *CurChar == '-') {

        char op = *CurChar;

        CurTok++;

        Element_t elem = {};

        if (op == '+')
            elem.Fdata = type_Add;
        else
            elem.Fdata = type_Sub;

        val = NewNode (elem, TFunc);

        Tree_node* val2 = GetT ();

        LinkNodes (val, val1, val2);

        val1 = val;

    }

    return val;

}

Tree_node* GetT () {

    Tree_node* val1 = GetPow ();
    Tree_node* val = val1;
 
    while (*CurChar == '*' || *CurChar == '/') {

        char op = *CurChar;

        CurTok++;

        Element_t elem = {};

        if (op == '*')
            elem.Fdata = type_Mul;
        else
            elem.Fdata = type_Div;

        val = NewNode (elem, TFunc);

        Tree_node* val2 = GetPow ();

        LinkNodes (val, val1, val2);

        val1 = val;

    }

    return val;


}

Tree_node* GetP () {

    if (*CurChar == '(') {

        CurTok++;
        Tree_node* val = GetE ();

        if (*CurChar != ')') {

           printf ("Error: invalid syntax near %.10s: expected ')'\n", CurChar);
           printf ("                           ^\n");
           return 0;

        }

        CurTok++;
        return val;

    }

    if (isdigit (*CurChar))
        return GetN ();

    return GetId ();

}

Tree_node* GetN () {

    if (CurTok -> NodeType != TNum)
        return nullptr;

    CurTok++;

    return (CurTok - 1);

}

Tree_node* GetId () {

    if (CurTok -> type != TId)
        return nullptr;

    CurTok++;

    return CurTok - 1;

}


///////////////////////////////////////////////////////////////////////////////////////////////

bool isspec (char ch) {

    if (ch == '=' || ch == '+' || ch == '-' || ch == '!' || ch == '*' || ch == '/' || ch == '|' || ch == '$' || ch == ':' || ch == '%' || ch == ']' || ch == '[')
        return true;

    return false;

}

Tree_node** Tokenization (char* text) {

    Tree_node** Tokens = (Tree_node**) calloc (Tree_node*, //TODO);

    CurChar = text;

    int CurToken = 0;

    while (*CurChar != EOF) {

        SkipSpace ();

        if (isalpha (*CurChar) || *CurChar == '_') {

            Tokens[CurToken] = TokWord ();
            CurToken++;

        }
        
        else

        if (isdigit (*CurChar)) {

            Tokens[CurToken] = TokNum ();
            CurToken++;

        }

        else

        if (isspec (*CurChar)) {

            Tokens[CurTok] = TokSpec ();
            CurToken++;

        }

        SkipSpace ();

    }

    CurTok = Tokens;

    return Tokens;

}

Tree_node* TokWord () {

    Tree_node* node = nullptr;

    char* word = GetWord ();
    len = strlen (word);

    switch (*word) {

    case 's': {

        if (strncmp (word, op[OP_sin], len) == 0)
            node = NewNode (OP_sin, TOp);

        else if (strncmp (word, op[OP_svol], len) == 0)
            node = NewNode (OP_svol, TOp);

        break;

    }

    case 'c': {

        if (strncmp (word, op[OP_cos], len) == 0)
            node = NewNode (OP_cos, TOp);

        break;

    }

    case 'm': {

        if (strncmp (word, op[OP_meno], len) == 0)
            node = NewNode (OP_meno, TOp);

        break;

    }

    case 'p': {

        if (strncmp (word, op[OP_piu], len) == 0)
            node = NewNode (OP_piu, TOp);

        break;
        
    }

    case 'v': {

        if (strncmp (word, op[OP_vol], len) == 0)
            node = NewNode (OP_vol, TOp);

        break;

    }

    case 'l': {

        if (strncmp (word, "legato", len))
            node = NewNode (D_leg, TDec);

        break;

    }

    case 'n': {

        if (strncmp (word, "nota", len))
            node = NewNode (D_nota, TDec);

        break;

    }

    if (node == nullptr)
        node = NewNode (word, TId);

    free (word);

    return node;

}

char* GetWord () {

    SkipSpace ();

    char* val = (char*) calloc (256, sizeof (char));
    int curSymb = 0;

    if (('a' <= *CurChar && *CurChar <= 'z') || ('A' <= *CurChar && *CurChar <= 'Z') || *CurChar == '_') {

        val[curSymb] = *CurChar;
        curSymb++;
        CurChar++;

    }

    else
        return 0;

    while (('a' <= *CurChar && *CurChar <= 'z') || ('A' <= *CurChar && *CurChar <= 'Z') || ('0' <= *CurChar && *CurChar <= '9') || *CurChar == '_') {

        val[curSymb] = *CurChar;
        curSymb++;
        CurChar++;

    }

    return val;

}

Tree_node* TokNum () {

    SkipSpace ();

    double val = 0;

    if ('0' <= *CurChar && *CurChar <= '9') {

        val = *CurChar - '0';
        CurChar++;

    }

    else {

        printf ("Error: invalid syntax near %.10s: expected number\n", CurChar);
        printf ("                           ^\n");
        return nullptr;

    }

    while ('0' <= *CurChar && *CurChar <= '9') {

        val = val * 10 + *CurChar - '0';
        CurChar++;

    }

    Element_t elem = {};
    elem.Ndata = val + GetMt();
    
    return NewNode (elem, TNum);

}

double GetMt () {

    double val = 0;
    double div = 10;

    if (*CurChar != '.')
          return 0;

    if ('0' <= *CurChar && *CurChar <= '9') {

        val = (*CurChar - '0') / div;
        div *= 10;
        CurChar++;

    }

    else {

        printf ("Error: invalid syntax near %.10s: expected number\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    while ('0' <= *CurChar && *CurChar <= '9') {

        val = val + (*CurChar - '0') / div;
        div *= 10;
        CurChar++;

    }
    
    return val;

}

Tree_node* TokSpec () {

    Tree_node* node = nullptr;

    switch (*CurChar) {

        case '=': {

            if (*(CurChar+1) == '=')
                node = NewNode (OP_eq, TOp);
_
            else node = NewNode (OP_ass, TOp);

            break;

        }

        case '$' : { 

            CurChar++; 
            node = TokNote (); 
            break; 
        
        }

        case '+' : {
 
            CurChar++;
            node = NewNode (OP_add, TOp); 
            break; 

        }

        case '-' : { 

            CurChar++;
            node = NewNode (OP_sub, TOp); 
            break; 
    
        }

        case '*' : { 

            CurChar++;
            node = NewNode (OP_mul, TOp); 
            break; 

        }

        case '/' : { 

            CurChar++;
            node = NewNode (OP_div, TOp); 
            break; 

        }

        case '%' : {

            CurChar++;
            node = NewNode (OP_mod, TOp);
            break;

        }

        case '|' : { 

            if (*(CurChar + 1) == '|')
                if (*(CurChar + 2) == ':') {

                    node = NewNode (OP_whl, TOp);
                    CurChar += 3;

                }

                else {

                    node = NewNode (OP_dbar, TOp);
                    CurChar += 2;
                
                }

            else {

                node = NewNode (OP_bar, TOp);
                CurChar++;

            }

            break;

         }

        case ':' : { 

            if ((*(CurChar + 1) != '|') || (*(CurChar +2) != '|'))
                return nullptr;

            node = NewNode (OP_clwhl, TOp);
            CurChar += 3; 

            break;

        }

        case ']' : {

            CurChar++;
            node = NewNode (OP_clbr, TOp);    
            break;

        }

        case '[' : {

            CurChar++;
            node = NewNode (OP_opbr, TOp);
            break;

        }

    }

    return node;

}

Tree_node* TokNote () {

    int val = 0;

    if ('H' < *CurChar || *CurChar < 'A')
        return nullptr;

    val += (*CurChar - 'A') * 100;

    CurChar++;

    if (*CurChar == 'b') {

        val += 10;
        CurChar++;

    }

    else if (*CurChar == '#') {

        val += 20;
        CurChar++;

    }

    if (!isdigit (*CurChar))
        return nullptr;

    val += *CurChar - '0';
    CurChar++;

    return NewNode (val, TNote);
        
}
