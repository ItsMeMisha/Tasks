#include "Tree_t.h"
#include <math.h>

void NodeDiffer (Tree_node* InNode, Tree_node* OutNode, Tree* OutTree);
void Differ (Tree* InTree, Tree* OutTree);

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

    #define NODE_TYPE( name, sign, num, code) \
            case type_##name: code; break;

    switch (InNode -> type) {

        #include "types.h"

    }

    return;

}

void Differ (Tree* InTree, Tree* OutTree) {

    ASSERTTREE (InTree);
    assert (InTree -> root);
    ASSERTTREE (OutTree);

    AddRoot (OutTree, 0);

    NodeDiffer (InTree -> root, OutTree -> root, OutTree);

    return;

}
