#include <math.h>
#include <stdlib.h>
#include "Tree_t.h"
#include "Comments.h"

const char LateXFile[] = "laTeX.latex";

void NodeDiffer (Tree_node* InNode, Tree_node* OutNode, Tree* OutTree);
void Differ (Tree* InTree, Tree* OutTree, FILE* file);
void Optimise (Tree* OutTree, Tree_node* branch, FILE* file);
void LateXNodeOut (FILE* file, Tree* tree, Tree_node* node);
void LateXTreeOut (FILE* file, Tree* tree);

int main () {

    Tree inputTree = {};
    TreeConstruct (&inputTree);

    Tree outputTree = {};
    TreeConstruct (&outputTree);

    FILE* file  = fopen ("input.txt", "r");
    ReadInfixTree (file, &inputTree);
    fclose (file);

    file = fopen (LateXFile, "w");

    fprintf (file, "\\documentclass[a4paper, 16pt]{article}\n \\begin{document}\n \\begin{math} \n");

    fprintf (file, "(");
    LateXTreeOut (file, &inputTree);
    fprintf (file, ")' = ");

    Differ (&inputTree, &outputTree, file);
   
    LateXTreeOut (file, &outputTree);

    fprintf (file, "\n \\end{math} \n \\end{document}\n");    

    fclose (file);

    TreeDestruct (&inputTree);
    TreeDestruct (&outputTree);

    return 0;

}

void NodeDiffer (Tree_node* InNode, Tree_node* OutNode, Tree* OutTree) {

    assert (InNode);
    assert (OutNode);
    ASSERTTREE (OutTree);

    #define DIFFER

    #define NODE_TYPE( name, signature, num, prior, code) \
            case type_##name: code; break;


    switch (InNode -> type) {

        #include "types.h"

    }

    #undef NODE_TYPE

    #undef DIFFER

    return;

}

void Differ (Tree* InTree, Tree* OutTree, FILE* file) {

    ASSERTTREE (InTree);
    assert (InTree -> root);
    ASSERTTREE (OutTree);

    AddRoot (OutTree, 0);

    Optimise (InTree, InTree -> root, file);

    NodeDiffer (InTree -> root, OutTree -> root, OutTree);

    Optimise (OutTree, OutTree -> root, file);

    fprintf (file, "\\newline");

    return;

}

void Optimise (Tree* OutTree, Tree_node* branch, FILE* file) {

    assert (file);

    if (branch -> left == nullptr && branch -> right == nullptr)
        return;
    
    if (branch -> left != nullptr)
        Optimise (OutTree, branch -> left, file);
    
    if (branch -> right != nullptr)
        Optimise (OutTree, branch -> right, file);

    int changes = 1;

    #define OPTIMISE

    #define NODE_TYPE(name, signature, num, prior, code) \
        case type_##name: { code; break; }

    bool ChangedAtLeastOneTime = false;

    fprintf (file, "\n\\newline %s", comment[rand ()% NumOfComments]);
    LateXTreeOut (file, OutTree);

    while (changes) {

        changes = 0;

        switch (branch -> type) {

            #include "types.h"

        }

        if (changes)
            ChangedAtLeastOneTime = true;

    }

    if (ChangedAtLeastOneTime) {

        fprintf (file, " = \\newline \n");
        LateXTreeOut (file, OutTree);

    }

    #undef NODE_TYPE

    #undef OPTIMISE

    return;

}

void LateXNodeOut (FILE* file, Tree* tree, Tree_node* node) {

    ASSERTTREE (tree);

    if (node -> type == type_Div)
        fprintf (file, "\\frac ");

    if (node -> left != nullptr) {

        if (node -> left -> priority >= node -> priority)
            fprintf (file, "{(");

        LateXNodeOut (file, tree, node -> left);

        if (node -> left -> priority >= node -> priority)
            fprintf (file, ")}");

    }

    if (node -> type == type_Mul)
        fprintf (file, "\\cdot ");

    else if (node -> type == type_Add || node -> type == type_Sub || node -> type == type_Pow || node -> type == type_Num || node -> type == type_Var)
        fprintf (file, "%s ", node -> data);

    else if (node -> type != type_Div)
        fprintf (file, "\\%s ", node -> data);

    if (node -> right != nullptr) {

        if (node -> right -> priority > node -> priority)
            fprintf (file, "{(");


        LateXNodeOut (file, tree, node -> right);

        if (node -> right -> priority > node -> priority)
            fprintf (file, ")}");

    }

    return;

}

void LateXTreeOut (FILE* file, Tree* tree) {

    ASSERTTREE (tree);


    if (tree -> root != nullptr)
        LateXNodeOut (file, tree, tree -> root);


    return;

}


