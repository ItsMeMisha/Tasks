#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include "Tree_t.h"

const size_t TokensNum = 10000;
const char* CurChar = "";
Tree_node** CurTok = nullptr;

void SkipSpace ();

Tree* GetTree (const char* fileName, Tree* tree);

Tree_node* GetG    (Tree_node** Tokens);
Tree_node* GetD    ();
Tree_node* GetFunc ();
Tree_node* GetFId  ();
Tree_node* GetVarlist ();
Tree_node* GetDef  ();
Tree_node* GetOp   ();
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
Tree_node* GetP    ();
Tree_node* GetN    ();
double     GetMt   ();
Tree_node* GetId   ();

bool isspec (char ch);
Tree_node** Tokenization (const char* text);
Tree_node*  TokWord ();
char*       GetWord ();
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
    char* Content = (char*) calloc (fileInfo.st_size + 1, sizeof (char));
    FILE* file = fopen (fileName, "r");

    if (fread (Content, sizeof (char), fileInfo.st_size, file) == 0)
        printf ("Ah shit!\n");;

    Content[fileInfo.st_size -1] = EOF;

    Tree_node** Tokens = Tokenization (Content);
    free (Content);

    tree -> root = GetG (Tokens);
    free (Tokens);

    return tree;

}

Tree_node* GetG (Tree_node** Tokens) {

    CurTok = Tokens;

    Tree_node* val = GetD ();

    while (val != nullptr) {

        val -> left = GetD ();
        val = val -> left;

    }

/*    if (*CurChar != '\0') {

        printf ("Error: invalid syntax near %.10s\n", CurChar);
        printf ("                           ^\n");
        return 0;

    } */

    return val;

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

    LinkNodes (val, GetVarlist (), val -> right);

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

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_clbr)
        return nullptr;

    while ((*CurTok) -> type == TId || (*CurTok) -> type == TNum) {

        LinkNodes (val, nullptr, *CurTok);
        
        CurTok++;

        if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_com) {

            LinkNodes (val, *CurTok, val -> right);
            val = val -> left;
            CurTok++;

        }
            
    }

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_clbr)
        return nullptr;

    return listbegin;

}

Tree_node* GetDef () {

    assert (*CurTok);

    CurTok++;
    assert (*CurTok);

    Tree_node* val = nullptr;

    while ((((*CurTok) -> data.Odata != OP_clwhl && (*CurTok) -> data.Odata != OP_dbar) && ((*CurTok) -> type == TOp)) || (*CurTok) -> type == TDec) {

        if (((*CurTok) -> type == TDec) && ((*CurTok) -> data.Ddata == D_nota)) {

            val = NewNode (D_dec, TDec);
            LinkNodes (val, nullptr, GetVar ());

        }

        else 

        LinkNodes (val, NewNode (D_dec, TDec), GetOp ());

        if (val -> right == nullptr)
            return nullptr;

        val = val -> left;

    }
//TODO
    return val;

}

Tree_node* GetOp () {

    Tree_node* val = nullptr;

    if ((val = GetVId ()) != nullptr) {

        assert (*CurTok);

        if ((*CurTok) -> type != TOp && (*CurTok) -> data.Odata != OP_ass)
            return nullptr;
 
    }

    else if ((val = GetFId ()) != nullptr) {

        LinkNodes (val, GetVarlist (), nullptr);

        if (val -> left == nullptr)
            return nullptr;

    }

    else if ((val = GetWhile ()) == nullptr);

    else if ((val = GetIf ()) == nullptr)
        return nullptr; 

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

    LinkNodes (val, GetInit (), val -> right);

    if (val -> left == nullptr)
        return nullptr;

    return val;

}

Tone_t GetTone () {

    assert (*CurTok);

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

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_whl)
        return nullptr;

    Tree_node* val = *CurTok;

    LinkNodes (val, GetCond (), nullptr);

    if ((*CurTok) -> data.Odata != OP_dbar || (*CurTok) -> type != TOp)
        return nullptr;

    CurTok++;

    LinkNodes (val, val -> left, GetDef ());

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata == OP_clwhl)
        return nullptr;

    CurTok++;

    return val;

}

Tree_node* GetIf () {

    assert (*CurTok);

    if ((*CurTok) -> type != TOp || (*CurTok) -> data.Odata != OP_vol)
        return nullptr;
    
    Tree_node* val = *CurTok;
    CurTok++;
    LinkNodes (val, GetCond (), nullptr);

    LinkNodes (val, val -> left, NewNode (OP_vol, TCon));

    if ((*CurTok) -> data.Odata != OP_dbar || (*CurTok) -> type != TOp)
        return nullptr;

    CurTok++;

    LinkNodes (val -> right, nullptr, GetDef ());

    if ((*CurTok) -> type == TOp && (*CurTok) -> data.Odata == OP_svol) {

        CurTok++;
        LinkNodes (val -> right, GetDef (), val -> right -> right);

    }

    return val;

}

Tree_node* GetCond () {

    assert (*CurTok);

    Tree_node* val1 = GetE ();
    Tree_node* val = GetComp ();
    Tree_node* val2 = GetE ();

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

    if ((*CurTok) -> type != TOp && (*CurTok) -> type != TNum)
        return nullptr;

    Tree_node* val1 = GetT ();

    Tree_node* val = val1;

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

    Tree_node* val1 = GetP ();
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

Tree_node* GetP () {

    assert (*CurTok);

    if ((*CurTok) -> data.Odata == OP_opbr) {

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


///////////////////////////////////////////////////////////////////////////////////////////////

bool isspec (char ch) {

    if (ch == '=' || ch == '+' || ch == '-' || ch == '!' || ch == '*' || ch == '/' || ch == '|' || ch == '$' || ch == ':' || ch == '%' || ch == ']' || ch == '[' || ch == ',')
        return true;

    return false;

}

Tree_node** Tokenization (const char* text) {

    Tree_node** Tokens = (Tree_node**) calloc (sizeof (Tree_node*), TokensNum);

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

            Tokens[CurToken] = TokSpec ();
            CurToken++;

        }

    }

    return Tokens;

}

Tree_node* TokWord () {

    Tree_node* node = nullptr;

    char* word = GetWord ();
    int len = strlen (word);

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

    case 'D': {

        if (strncmp (word, op[OP_ds], len) == 0)
            node = NewNode (OP_ds, TOp);

        break;

    }

    case 'l': {

        if (strncmp (word, "legato", len) == 0)
            node = NewNode (D_leg, TDec);

       break;

    }

    case 'n': {

        if (strncmp (word, "nota", len) == 0)
            node = NewNode (D_nota, TDec);

        break;

    }

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

    double num = val + GetMt();
    
    return NewNode (num, TNum);

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

            CurChar++;

            if (*CurChar == '=') {

                node = NewNode (OP_eq, TOp);
                CurChar++;

            }

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

        case ',': {

            CurChar++;
            node = NewNode (OP_com, TOp);
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
