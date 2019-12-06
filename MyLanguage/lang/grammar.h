#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>
#include "Tree_t.h"

const char* CurChar = "";

void SkipSpace ();

Tree* GetTree (const char* fileName, Tree* tree);

Tree_node* GetG   (const char* str);
Tree_node* GetD   ();
Tree_node* GetFunc();
Tree_node* GetFId ();
Tree_node* GetVarlist ();
Tree_node* Def    ();
Tree_node* Op     ();
Tree_node* Var    ();
Tree_node* VId    ();
Tree_node* Init   ();
Tree_node* GetWhile();
Tree_node* GetIf  ();
Tree_node* GetCond ();
Tree_node* GetComp ();
Tree_node* GetE   ();
Tree_node* GetT   ();
Tree_node* GetPow ();
Tree_node* GetP   ();
Tree_node* GetN   ();
double     GetMt  ();
Tree_node* GetId  ();

Tree_node** Tokenization (char* text);

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

    SkipSpace ();

    Tree_node* val = nullptr;

    if (strncmp (CurChar, "legato", 6) == 0) {

        val = NewNode ();
        CurChar += 6;
        val -> right = GetFunc ();

    }

    else if (strncmp (CurChar, "nota", 4) == 0) {

        val = NewNode ();
        CurChar += 4;
        val -> right = GetVar ();

    }

    else return nullptr;

    return val;

}

Tree_node* GetFunc () {

    SkipSpace ();
//TODO
    Tree_node* val = NewNode ();

    if ((val -> left = GetVarlist ()) == nullptr)
        return nullptr;

    if ((val -> right = DetFId ()) == nullptr)
        return nullptr;

    if ((val -> right -> right = Def ()) == nullptr)
        return nullptr;

    return val;

}

Tree_node* GetFId () {

    SkipSpace ();

    retunr val;

}

Tree_node* GetVarlist () {

    SkipSpace ();

    Tree_node* val = NewNode ();

    while

    return val;

}

Tree_node* Def () {

    SkipSpace ();

    if (*CurChar != '|' || *(CurChar + 1) != '|')
        return nullptr;

    CurChar += 2;

    while (*CurChar != '|') {

        if (strncmp (CurChar, "nota", 4) == 0) {

            val = NewNode ();
            CurChar += 4;
            val -> right = GetVar ();

        }

        else val -> right = GetOp ();
    
        val -> left = NewNode ();
        val = val -> left;

        SkipSpace ();

    }

    CurChar++;
    
    if (*CurChar != '|') return nullptr;

    CurChar++;

    return val;

}

Tree_node* Op () {

    SkipSpace ();

    Tree_node* val = nullptr;

    if ((val = VId ()) != nullptr) {

        SkipSpace ();
        if (*CurChar != '=')
            return nullptr;

        CurChar++;

        //TODO

    }

    else if ((val = GetFId ()) != nullptr) {

        SkipSpace ();
        if ((/*TODO*/ = GetVarlist ()) == nullptr)
            return nullptr;

    }

    else if ((val = GetWhile ()) == nullptr)

    if ((val = GetIf ()) == nullptr)
        return nullptr; 

    return val;

}

Tree_node* Var () {

    SkipSpace ();

    Tree_node* val = NewNode ();
    
    val -> left = GetVId ();

    if (val -> left == nullptr) {

        free (val);
        return nullptr;

    }

    val -> right = GetInit ();

    return val;

}

Tree_node* VId () {

    SkipSpace ();

    retunr val;

}

Tree_node* Init () {

    SkipSpace ();

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

    if (strncmp (CurChar, "volta", 5) != 0)
        return nullptr;

    CurChar += 5;
    
    Tree_node* val = NewNode ();

    val -> left = GetCond ();
    val -> right = NewNode ();
    val -> right -> right = GetDef;

    SkipSpace ();

    if (strncmp (CurChar, "svolta", 6) == 0) {

        CurChar += 6;
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

    SkipSpace ();

    Tree_node* val = nullptr;

    if (*CurChar == '<')
        val = NewNode ();

    else if (*CurChar == '>')
        val = NewNode ();

    else if (*(CurChar + 1)  == '=') {

        CurChar++;

        if (*(CurChar - 1) == '!')
            val = NewNode ();

        else if (*(CurChar - 1) == '=')
            val = NewNode ();

    }

    else return nullptr;

    CurChar++;

    return val;

}

Tree_node* GetE () {

    SkipSpace ();

    Tree_node* val1 = GetT ();

    Tree_node* val = val1;

    while (*CurChar == '+' || *CurChar == '-') {

        char op = *CurChar;

        CurChar++;

        Element_t elem = {};

        if (op == '+')
            elem.Fdata = type_Add;
        else
            elem.Fdata = type_Sub;

        val = NewNode (elem, TFunc);

        Tree_node* val2 = GetT ();

        val -> left = val1;
        val1 -> parent = val; 
        val -> right = val2;
        val2 -> parent = val;
        val1 = val;

    }

    return val;

}

Tree_node* GetT () {

    SkipSpace ();

    Tree_node* val1 = GetPow ();
    Tree_node* val = val1;
 
    while (*CurChar == '*' || *CurChar == '/') {

        char op = *CurChar;

        CurChar++;

        Element_t elem = {};

        if (op == '*')
            elem.Fdata = type_Mul;
        else
            elem.Fdata = type_Div;

        val = NewNode (elem, TFunc);

        Tree_node* val2 = GetPow ();

        val -> left = val1;
        val1 -> parent = val; 
        val -> right = val2;
        val2 -> parent = val;
        val1 = val;

    }

    return val;


}

Tree_node* GetPow () {

    SkipSpace ();

    Tree_node* val1 = GetP ();
    Tree_node* val = val1;
 
    while (*CurChar == '^') {

        char op = *CurChar;

        CurChar++;

        Element_t elem = {};
        elem.Fdata = type_Pow;

        val = NewNode (elem, TFunc);

        Tree_node* val2 = GetP ();

        val -> left = val1;
        val1 -> parent = val; 
        val -> right = val2;
        val2 -> parent = val;
        val1 = val;

    }

    return val;

}

Tree_node* GetP () {

    SkipSpace ();

    if (*CurChar == '(') {

        CurChar++;
        Tree_node* val = GetE ();

        if (*CurChar != ')') {

           printf ("Error: invalid syntax near %.10s: expected ')'\n", CurChar);
           printf ("                           ^\n");
           return 0;

        }

        CurChar++;
        return val;

    }

    if (isdigit (*CurChar))
        return GetN ();

    return GetId ();

}

Tree_node* GetN () {

    SkipSpace ();

    double val = 0;

    if ('0' <= *CurChar && *CurChar <= '9') {

        val = *CurChar - '0';
        CurChar++;

    }

    else {

        printf ("Error: invalid syntax near %.10s: expected number\n", CurChar);
        printf ("                           ^\n");
        return 0;

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

Tree_node* GetId () {

    SkipSpace ();

    char* val = (char*) calloc (256, sizeof (char));
    int curSymb = 0;

    if (('a' <= *CurChar && *CurChar <= 'z') || ('A' <= *CurChar && *CurChar <= 'Z') || *CurChar == '_') {

        val[curSymb] = *CurChar;
        curSymb++;
        CurChar++;

    }

    else {

        printf ("Error: invalid syntax near %.10s: expected number\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    while (('a' <= *CurChar && *CurChar <= 'z') || ('A' <= *CurChar && *CurChar <= 'Z') || ('0' <= *CurChar && *CurChar <= '9') || *CurChar == '_') {

        val[curSymb] = *CurChar;
        curSymb++;
        CurChar++;

    }

    Element_t type = FuncDef (val);

    free (val);
    
    if (type.Fdata == type_Var)
        return NewNode (type, TVar);
    
    Tree_node* value = NewNode (type, TFunc);
    value -> right = GetP ();

    return value;

}

Tree_node** Tokenization (char* text) {

    Tree_node** Tokens = (Tree_node**) calloc (Tree_node*, //TODO);

    



    return Tokens;

}
