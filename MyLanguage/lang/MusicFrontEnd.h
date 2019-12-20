#ifndef __MUSICFRONTEND__

    #define __MUSICFRONTEND__

#include <stdio.h>
#include "grammar.h"
#include "funcs_and_vars.h"

short counter = 0;

void DefineVarTone (Tree_node* node);
void DefineGVarTone (Vars* vars, Tree_node* node);
void DefineFVarTone (AvVars* vars, Tree_node* node);
char* ConvertTone (int tone);
void PrintMusic (FILE* file, Tree* tree);
void PrMusNode (FILE* file, Tree_node* node);

void DefineVarTone (Tree_node* node) {

    assert (node);

    assert (node -> right);

    Vars* GVars = ConstructVars (nullptr);

    while (node != nullptr && node -> right != nullptr && node -> right -> type == TDec) {

        if (node -> right -> data.Ddata == D_leg) {

            AvVars* vars = ConstructAvVars (nullptr, 0, GVars);
            DefineFVarTone (vars, node -> right);
            DestructVars (vars -> fVars);
            free (vars); 

        }

        else if (node -> right -> data.Ddata == D_nota)
            DefineGVarTone (GVars, node -> right);

        node = node -> left;
        printf ("\r");
    }

    DestructVars (GVars);

    return;


}

void DefineGVarTone (Vars* vars, Tree_node* node) {

    assert (vars);

    if (node -> type != TDec || node -> data.Ddata != D_nota)
        return;

    assert (node -> right);

    if (node -> right -> type != TId)
        return;

    int ton = -1;

    if ((ton = FindVar (vars, node -> right -> data.Sdata)) >= 0)
        return;

    ton = node -> right -> tone;

    AddVar (vars, node -> right -> data.Sdata, ton);

    return;

}

void DefineFVarTone (AvVars* vars, Tree_node* node) {

    assert (vars);
    
    if (node == nullptr)
        return; 

    int ton = -1;

    if (node -> type == TId) {

        if ((ton = FindVar (vars -> fVars, node -> data.Sdata)) >= 0) 
            node -> tone = vars -> fVars -> var[ton] -> pos;

        else if ((ton = FindVar (vars -> gVars, node -> data.Sdata)) >= 0)
            node -> tone = vars -> gVars -> var[ton] -> pos;

        else {

            ton = node -> tone;
            AddVar (vars -> fVars, node -> data.Sdata, ton);

        }

    }

    if (node -> type == TDec && node -> data.Ddata == D_leg) {

        DefineFVarTone (vars, node -> left);
        DefineFVarTone (vars, node -> right);

    }
    
    else {

        DefineFVarTone (vars, node -> right);
        DefineFVarTone (vars, node -> left);

    }

    return;

}

char* ConvertTone (int tone) {

    if ((tone / 1000) != 0)
        return nullptr;

    int ind = 0;

    char* nota  = (char*) calloc (sizeof (char), 16);;

    if ((tone / 10) % 10 == 1) {

        nota[ind] = '_';
        ind++;

    }

    else if ((tone / 10) % 10 == 2) {

        nota[ind] = '^';
        ind++;

    }

    int notenum = tone / 100 - 1;

    int oct = tone % 10;

    if (oct <= 5) {

        nota[ind] = 'A' + notenum;
        ind++;

        for (oct; oct < 5; oct++) {

            nota[ind] = ',';
            ind++;

        }

    }

    else {

        nota[ind] = 'a' + notenum;
        ind++;

        for (oct; oct > 6; oct--) {

            nota[ind] = '`';
            ind++;

        }
    
    }  

    return nota;

}

void PrintMusic (FILE* file, Tree* tree) {

    fprintf (file, "X: 1 \nM: 4/4 \nL: 1/8\nK: C\n |:");

    assert (tree);
    assert (tree -> root);

    DefineVarTone (tree -> root);

    PrMusNode (file, tree -> root);

    fprintf (file, "!fermata!:|");
    
    return;

}

void PrMusNode (FILE* file, Tree_node* node) {

    assert (node);

    if (node -> left != nullptr)
        PrMusNode (file, node -> left);

    if (node -> right != nullptr)
        PrMusNode (file, node -> right);

    if (node -> tone > 0) {

        char* note = ConvertTone (node -> tone);

        if (counter == 0)
            fprintf (file, "[%s,%s]2 ", note, note);

        else fprintf (file, "%s ", note);

        if (counter > 5) {

            fprintf (file, "|");
            counter = 0;

        }

        else counter++;

        free (note);
   
    }

    return;

}

#endif
