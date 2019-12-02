#include <math.h>
#include <stdlib.h>
#include "Tree_t.h"
#include "Comments.h"
#include "grammar.cpp"

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

    GetTree ("input.txt", &inputTree);

    FILE* file = fopen (LateXFile, "w");

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
 
    #define SET(Out, newdata, newtype, prior) Out -> data = (Element_t) {newdata}; Out -> type = newtype; Out -> priority = prior;
   
    if (InNode -> type == TVar) {
        SET (OutNode, 1, TNum, 0); }

    else if (InNode -> type == TNum) {
        SET (OutNode, 0, TNum, 0); }

    else  

    #undef SET

    switch (InNode -> data.Fdata) {

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
TreeDump (InTree);

    AddRoot (OutTree, (Element_t) {0});

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

        if (branch -> type == TFunc) {

            switch (branch -> data.Fdata) {

                #include "types.h"

            }

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

    if (node -> data.Fdata == type_Div)
        fprintf (file, "\\frac ");

    if (node -> left != nullptr) {

        if (node -> left -> priority > node -> priority)
            fprintf (file, "{(");

        LateXNodeOut (file, tree, node -> left);

        if (node -> left -> priority > node -> priority)
            fprintf (file, ")}");

    }

    if (node -> type == TNum) 
        fprintf (file, "%lg ", node -> data.Ndata);

    else

    if (node -> data.Fdata == type_Mul)
        fprintf (file, "\\cdot ");

    else if (node -> data.Fdata == type_Add || node -> data.Fdata == type_Sub || node -> data.Fdata == type_Pow || node -> data.Fdata == type_Num || node -> data.Fdata == type_Var)
        fprintf (file, "%s ", func[node -> data.Fdata]);

    else if (node -> data.Fdata != type_Div)
        fprintf (file, "\\%s ", func[node -> data.Fdata]);

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


