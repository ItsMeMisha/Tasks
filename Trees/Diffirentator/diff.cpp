#include <math.h>
#include <stdlib.h>
#include "Tree_t.h"

void NodeDiffer (Tree_node* InNode, Tree_node* OutNode, Tree* OutTree);
void Differ (Tree* InTree, Tree* OutTree);
void Optimise (Tree* OutTree, Tree_node* branch);

int main () {

    Tree inputTree = {};
    TreeConstruct (&inputTree);
    Tree outputTree = {};
    TreeConstruct (&outputTree);

    FILE* file  = fopen ("input.txt", "r");
    ReadInfixTree (file, &inputTree);
    fclose (file);

    Differ (&inputTree, &outputTree);
    
    file = fopen ("output.txt", "w");
    PrintInfixTree (file, &outputTree);
    fclose (file);

    TreeDestruct (&inputTree);
    TreeDestruct (&outputTree);

    return 0;

}

void NodeDiffer (Tree_node* InNode, Tree_node* OutNode, Tree* OutTree) {

    assert (InNode);
    assert (OutNode);
    ASSERTTREE (OutTree);

    #define NODE_TYPE( name, sign, num, code, opt) \
            case type_##name: code; break;


    switch (InNode -> type) {

        #include "types.h"

    }

    #undef NODE_TYPE

    return;

}

void Differ (Tree* InTree, Tree* OutTree) {

    ASSERTTREE (InTree);
    assert (InTree -> root);
    ASSERTTREE (OutTree);

    AddRoot (OutTree, 0);

    Optimise (InTree, InTree -> root);

    NodeDiffer (InTree -> root, OutTree -> root, OutTree);

    return;

}

void Optimise (Tree* OutTree, Tree_node* branch) {

    if (branch -> left == nullptr && branch -> right == nullptr)
        return;
    
    if (branch -> left != nullptr)
        Optimise (OutTree, branch -> left);
    
    if (branch -> right != nullptr)
        Optimise (OutTree, branch -> right);

    int changes = 1;

    #define NODE_TYPE(name, signaure, num, code, optimise) \
        case type_##name: optimise; break;

    while (changes) {

        changes = 0;

        switch (branch -> type) {

            #include "types.h"

        }

    }

    return;

}


