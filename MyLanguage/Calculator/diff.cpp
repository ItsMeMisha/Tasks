#include <math.h>
#include <stdlib.h>
#include "Tree_t.h"
#include "Comments.h"
#include "grammar.h"

const char LateXFile[] = "laTeX.latex";

void LatexHeader (FILE* file);
Tree_node* NodeDiffer (Tree_node* InNode, Tree* OutTree, FILE* file);
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

    LatexHeader (file);

    Differ (&inputTree, &outputTree, file);
   
    fprintf (file, "\n \\end{math} \n \\end{document}\n");    

    fclose (file);
    system ("pdflatex laTeX.latex -o laTeX.pdf");

    TreeDestruct (&inputTree);
    TreeDestruct (&outputTree);

    return 0;

}

void LatexHeader (FILE* file) {

    fprintf (file, "\\documentclass[a4paper,16pt]{article} \n");
    fprintf (file, "\\title {Derivative} \n");
    fprintf (file, "\\begin{document} \n \\maketitle \n \\begin{math} \n");

    return;

}

Tree_node* NodeDiffer (Tree_node* InNode, Tree* OutTree, FILE* file) {

    assert (InNode);
    ASSERTTREE (OutTree);
 
    Tree_node* OutNode = nullptr;
 
    if (InNode -> type == TVar)
        OutNode = NewNode (1, TNum, nullptr, nullptr);

    else if (InNode -> type == TNum) 
        OutNode = NewNode (0, TNum, nullptr, nullptr);

    else  

    #define DIFFER

    #define NODE_TYPE( name, signature, num, prior, code) \
            case type_##name: code; break;
 
    switch (InNode -> data.Fdata) {

        #include "types.h"

    }

    #undef NODE_TYPE

    #undef DIFFER

    OutTree -> root = OutNode;

    fprintf (file, "\\newline %s (", comment[rand ()% NumOfComments]);
    LateXNodeOut (file, OutTree, InNode);
    fprintf (file, ")' = ");
 
    LateXNodeOut (file, OutTree, OutNode);
    fprintf (file, "\\newline\n");

    return OutNode;

}

void Differ (Tree* InTree, Tree* OutTree, FILE* file) {

    ASSERTTREE (InTree);
    assert (InTree -> root);
    ASSERTTREE (OutTree);

    AddRoot (OutTree, (Element_t) {0});


    fprintf (file, "Ok,\\ we\\ have\\ ");
    LateXTreeOut (file, InTree);
    fprintf (file, "\\newline\n");

    Optimise (InTree, InTree -> root, file);

    OutTree -> root = NodeDiffer (InTree -> root, OutTree, file);

    TreeDump (OutTree);

    LateXTreeOut (file, OutTree);

    Optimise (OutTree, OutTree -> root, file);

    fprintf (file, "\n\\newline \\newline And\\ so\\ we\\ have\\ (");
    LateXTreeOut (file, InTree);
    fprintf (file, ")' = ");
    LateXTreeOut (file, OutTree);

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

    bool ChangedAtLeastOneTime = false;

    while (changes) {

        changes = 0;

        if (branch -> type == TFunc) {

        #define OPTIMISE

        #define NODE_TYPE(name, signature, num, prior, code) \
            case type_##name: { code; break; }

            switch (branch -> data.Fdata) {

                #include "types.h"

            }

        }

        #undef NODE_TYPE

        #undef OPTIMISE

        if (changes)
            ChangedAtLeastOneTime = true;

    }

    if (ChangedAtLeastOneTime) {

        fprintf (file, " = ");
        LateXTreeOut (file, OutTree);

    }

    return;

}

void LateXNodeOut (FILE* file, Tree* tree, Tree_node* node) {

    ASSERTTREE (tree);

    if (node -> data.Fdata == type_Div)
        fprintf (file, "\\frac ");

    if (node -> left != nullptr) {

        if (node -> left -> priority >= node -> priority) {

            fprintf (file, "{(");
            LateXNodeOut (file, tree, node -> left);
            fprintf (file, ")}");
        
        }

        else {

            fprintf (file, "{");
            LateXNodeOut (file, tree, node -> left);
            fprintf (file, "}");

        }

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

        if (node -> right -> priority > node -> priority) {

            fprintf (file, "{(");
            LateXNodeOut (file, tree, node -> right);
            fprintf (file, ")}");

        }

        else {

            fprintf (file, "{");
            LateXNodeOut (file, tree, node -> right);
            fprintf (file, "}");

        }

    }

    return;

}

void LateXTreeOut (FILE* file, Tree* tree) {

    ASSERTTREE (tree);

    if (tree -> root != nullptr) {

//        fprintf (file, "$");
        LateXNodeOut (file, tree, tree -> root);
//        fprintf (file, "$");

    }

    return;

}


