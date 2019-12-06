#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>
#include "Tree_t.h"

const char* CurChar = "";

void SkipSpace ();

Tree* GetTree (const char* fileName, Tree* tree);

Tree_node* GetG   (const char* str);
Tree_node* GetE   ();
Tree_node* GetT   ();
Tree_node* GetPow ();
Tree_node* GetP   ();
Tree_node* GetN   ();
double     GetD   ();
Tree_node* GetId  ();

void SkipSpace () {

    while (*CurChar == ' ')
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

    CurChar = str;
        
    Tree_node* val = GetE ();

    if (*CurChar != '\n') {

        printf ("Error: invalid syntax near %.10s\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    return val;

}

Tree_node* GetE () {

    SkipSpace ();

    Tree_node* val1 = GetT ();

    Tree_node* val = val1;

    while (*CurChar == '+' || *CurChar == '-') {

        char op = *CurChar;

        CurChar++;

        if (op == '+')
            val = NewNode (type_Add, TFunc);
        else
            val = NewNode (type_Sub, TFunc);

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

        if (op == '*')
            val = NewNode (type_Mul, TFunc);
        else
            val = NewNode (type_Div, TFunc);

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

        val = NewNode (type_Pow, TFunc);

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
   
    return NewNode (val + GetD (), TNum);

}

double GetD () {

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
        return NewNode (type.Fdata, TVar);
    
    Tree_node* value = NewNode (type.Fdata, TFunc);
    value -> right = GetP ();

    return value;

}


