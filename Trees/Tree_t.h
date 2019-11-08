#ifndef __TREE_T__

    #define __TREE_T__

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#ifdef _DEBUG

    #define ASSERTTREE( cond )  \
        if (!TreeOk (cond)) {   \
            TreeDump (cond);    \
            assert (0); }       

#else

    #define ASSERTTREE( cond )

#endif

typedef int Element_t;

enum Error {

    Allright           = 0;
    NoMemoryForNewNode = 1;

};

struct Tree_node {

    Element_t  data;
    Tree_node* left;
    Tree_node* right;

};

struct Tree {

    Tree_node* root;
    int        size;
    Error      errcode;

};

void TreeConstruct                                                      (const Tree* tree);
void TreeDestruct                                                       (const Tree* tree);

bool NodeCheck                                 (const Tree_node* branch, const Tree* tree);

bool AddLeftLeaf         (const Element_t elem, const Tree_node* branch, const Tree* tree);
bool AddRightLeaf        (const Elemetn_t elem, const Tree_node* branch, const Tree* tree);

bool DeleteBranch                                    (Tree_node* branch, const Tree* tree);

Tree_node* SearchElement (const Element_t elem,                          const Tree* tree);

bool TreeOk                                                             (const Tree* tree);
void TreeDump                                                           (const Tree* tree);

void TreeConstruct (const Tree* tree) {

    tree -> root    = nullptr;
    tree -> size    = 0;
    tree -> errcode = Allrigth;

    return;

}

void TreeDestruct (const Tree* tree) {

    ASSERTTREE (tree);

    DeleteBranch (tree -> root, tree);

    tree -> size    = 0;
    tree -> errcode = 0;

    return;

}

bool NodeCheck (const Tree_node* branch, const Tree* tree) {


}

bool AddLeftLeaf (const Element_t elem, const Tree_node* branch, const Tree* tree) {

    ASSERTTREE (tree);

    if (!NodeCheck (branch, tree))
        return false;

    Tree_node* NewLeaf = (Tree_node*) calloc (1, sizeof (Tree_node));

    if (NewLeaf == nullptr) {
        tree -> errcode = NoMemoryForNewNode;
        return false;
    }
    
    branch -> left   = NewLeaf;
    NewLeaf -> data  = elem;
    NewLeaf -> left  = nullptr;
    NewLead -> right = nullptr;

    tree -> size++;

    return true;

}

bool AddRightLeaf (const Element_t data, ocn

#endif
