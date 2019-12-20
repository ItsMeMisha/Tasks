#ifndef __MIDDLEEND__

    #define __MIDDLEEND__

#include <stdio.h>
#include <math.h>
#include "Tree_t.h"

void OptimiseTree (Tree* tree);
void OptimiseNode (Tree_node* node);
void OptimiseTwoArgs (Tree_node* node);
void OptimiseOneArg  (Tree_node* node);
 
void OptimiseTree (Tree* tree) {

    assert (tree);
    assert (tree -> root);

    OptimiseNode (tree -> root);

    return;

}

void OptimiseNode (Tree_node* node) {

    assert (node);

    if (node -> left != nullptr)
        OptimiseNode (node -> left);


    if (node -> right != nullptr) {

        OptimiseNode (node -> right);
        if (node -> right -> type != TNum)
            return;

    } else return;

    if (node -> type != TOp)
        return;

    if (node -> left == nullptr) 
        OptimiseOneArg (node);

    else if (node -> left -> type == TNum)
        OptimiseTwoArgs (node);

    return;
    
}

void OptimiseTwoArgs (Tree_node* node) {

    assert (node);
    assert (node -> left);
    assert (node -> right);

    switch (node -> data.Odata) {

        case OP_add: node -> type = TNum; 
                     node -> data.Ndata = node -> right -> data.Ndata + node -> left -> data.Ndata; break;
        case OP_sub: node -> type = TNum; 
                     node -> data.Ndata = node -> left -> data.Ndata - node -> right -> data.Ndata; break;
        case OP_mul: node -> type = TNum; 
                     node -> data.Ndata = node -> left -> data.Ndata * node -> right -> data.Ndata; break;
        case OP_div: node -> type = TNum; 
                     node -> data.Ndata = node -> left -> data.Ndata / node -> right -> data.Ndata; break;

        default: return;

    }

    free (node -> right);
    node -> right = nullptr;
    free (node -> left);
    node -> left = nullptr;

    return;

}

void OptimiseOneArg  (Tree_node* node) {

    assert (node);
    assert (node -> right);

    switch (node -> data.Odata) {

        case OP_sin: node -> type = TNum; node -> data.Ndata = sin (node -> right -> data.Ndata); break;

        case OP_cos: node -> type = TNum; node -> data.Ndata = cos (node -> right -> data.Ndata); break;

        case OP_sqrt: node -> type = TNum; node -> data.Ndata = sqrt (node -> right -> data.Ndata); break;

        default: return;

    }

    free (node -> right);
    node -> right = nullptr;

    return;

}

#endif
