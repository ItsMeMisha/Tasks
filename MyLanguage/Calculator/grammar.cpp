#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>
#include "Tree_t.h"

const char* CurChar = "";

Tree* GetTree (const char* fileName, Tree* tree);

Tree_node* GetG  (const char* str);
Tree_node* GetE  ();
Tree_node* GetT  ();
Tree_node* GetP  ();
Tree_node* GetN  ();
double     GetD  ();
Tree_node* GetId ();

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

/*    if (*CurChar != '\0') {

        printf ("Error: invalid syntax near %.10s\n", CurChar);
        printf ("                           ^\n");
        return 0;

    } */

    return val;

}

Tree_node* GetE () {

    Tree_node* val1 = GetT ();

    Tree_node* val = val1;

    if (*CurChar == '+' || *CurChar == '-') {

        char op = *CurChar;

        CurChar++;

        Element_t elem = {};

        if (op == '+')
            elem.Fdata = type_Add;
        else
            elem.Fdata = type_Sub;

        val = NewNode (elem, TFunc);

        Tree_node* val2 = GetE ();

        val -> left = val1;
        val1 -> parent = val; 
        val -> right = val2;
        val2 -> parent = val;

    }

    return val;

}

Tree_node* GetT () {

    Tree_node* val1 = GetP ();
    Tree_node* val = val1;
 
    if (*CurChar == '*' || *CurChar == '/') {

        char op = *CurChar;

        CurChar++;

        Element_t elem = {};

        if (op == '*')
            elem.Fdata = type_Mul;
        else
            elem.Fdata = type_Div;

        val = NewNode (elem, TFunc);

        Tree_node* val2 = GetT ();

        val -> left = val1;
        val1 -> parent = val; 
        val -> right = val2;
        val2 -> parent = val;

    }

    return val;


}

Tree_node* GetP () {

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
    elem.Ndata = val + GetD();
    
    return NewNode (elem, TNum);

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


