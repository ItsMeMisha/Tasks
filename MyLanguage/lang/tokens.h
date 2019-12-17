#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include "Tree_t.h"

const size_t TokensNum = 10000;
const char* CurChar = "";

void SkipSpace ();
bool isspec (char ch);
Tree_node** Tokenization (const char* text);
Tree_node*  TokWord ();
char*       GetWord ();
Tree_node*  TokNum  ();
Tree_node*  TokSpec ();
Tree_node*  TokNote ();
double     GetMt   ();

void SkipSpace () {

    while (*CurChar == ' ' || *CurChar == '\n' || *CurChar == '\r' || *CurChar == '\t')
        CurChar++;

    return;

}

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
    int len = strlen (word) + 1;

    switch (*word) {

    case 's': {

        if (strncmp (word, op[OP_sin], len) == 0)
            node = NewNode (OP_sin, TOp);

        else if (strncmp (word, op[OP_svol], len) == 0)
            node = NewNode (OP_svol, TOp);
    
        else if (strncmp (word, op[OP_sqrt], len) == 0)
            node = NewNode (OP_sqrt, TOp);

        else if (strncmp (word, op[OP_prnt], len) == 0)
            node = NewNode (OP_prnt, TOp);

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

        else if (strncmp (word, op[OP_read], len) == 0)
            node = NewNode (OP_read, TOp);

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

    val += (*CurChar - 'A' + 1) * 100;

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
