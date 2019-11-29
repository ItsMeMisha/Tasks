#include <stdio.h>
#include <malloc.h>
#include "Tree_t.h"

const char* CurChar = "";

Tree* GetTree (FILE* file, Tree* tree);

Tree_node* GetG (const char* str);
Tree_node* GetE ();
Tree_node* GetT ();
Tree_node* GetP ();
Tree_node* GetN ();
double     GetD ();
Tree_node* GetX ();

Tree* GetTree (FILE* file, Tree* tree) {

    struct stat fileInfo = {};
    stat (file, &fileinfo);
    char* Content = (char*) calloc (fileInfo.st_size , sizeof (char));
    Content = fread (Content, sizeof (char), fileinfo.size, file);
    
    tree -> root = GetG (Content);

    free (Content);

    return tree;

}

Tree_node* GetG (const char* str) {

    CurChar = str;
        
    Tree_node* val = GetE ();

    if (*CurChar != '\0') {

        printf ("Error: invalid syntax near %.10s\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    return val;

}

Tree_node* GetE () {

    Tree_node* val1 = GetT ();

    Tree_node* val = val1;

    if (*CurChar == '+' || *CurChar == '-') {

        char op = *CurChar;

        CurChar++;

        if (op == '+')
            val = NewNode ("+");
        else
            val = NewNode ("-");

        Tree_node* val2 = GetE ();

        val -> left = val1; 
        val -> right = val2;

    }

    return val;

}

Tree_node* GetT () {

    Tree_node* val1 = GetP ();
    Tree_node* val = val1;
 
    if (*CurChar == '*' || *CurChar == '/') {

        char op = *CurChar;

        CurChar++;

        if (op == '*')
            val = NewNode ("*");
        else
            val = NewNode ("/");

        Tree_node* val2 = GetT ();

        val -> left  = val1;
        val -> right = val2;

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

    return GetN ();

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
    
    return NewNode (val + GetD);

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

Tree_node* GetX () {

    char* val = (char*) calloc (256, sizeof (char));
    int curSymb = 0;

    if (('a' <= *CurChar && *CurChar <= 'z') || ('A' <= *CurChar && *CurChar <= 'Z')) {

        val[curSymb] = *CurChar;
        curSymb++;
        CurChar++;

    }

    else {

        printf ("Error: invalid syntax near %.10s: expected number\n", CurChar);
        printf ("                           ^\n");
        return 0;

    }

    while (('a' <= *CurChar && *CurChar <= 'z') || ('A' <= *CurChar && *CurChar <= 'Z')) {

        val[curSymb] = *CurChar;
        curSymb++;
        CurChar++;

    }
    
    return NewNode ({type_Num, {val}, 10});

}


