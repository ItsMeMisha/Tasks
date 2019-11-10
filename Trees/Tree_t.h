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
    Tree_node* parent;
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

bool ParentsCheck                              (const Tree_node* branch, const Tree* tree, const int deep = 0);

Tree_node* NewNode       (const Element_t elem);
bool Add_Unlinked_Leaf_Very_Very_Dangerous_Only_For_Developer_Of_This_Tree (const Element_t elem, const Tree_node* branch, const Tree* tree);
bool AddLeftLeaf         (const Element_t elem, const Tree_node* branch, const Tree* tree);
bool AddRightLeaf        (const Elemetn_t elem, const Tree_node* branch, const Tree* tree);

bool DeleteBranch                                    (Tree_node* branch, const Tree* tree);

Tree_node* SearchElement (const Element_t elem,                          const Tree* tree);

void PrintPreTree                                                       (const Tree* tree);
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

bool ParentsCheck (const Tree_node* branch, const Tree* tree, const int deep) {

    if (deep > tree -> size) {
        return false;

    if (branch == tree -> root)
        return true;

    if (branch -> parent == nullptr)
        return false;

    return ParentsCheck (branch -> parent, tree, deep + 1);

}

Tree_node* NewNode (const Element_t elem) {

    Tree_node* NewLeaf = (Tree_node*) calloc (1, sizeof (Tree_node));

    if (NewLeaf == nullptr)
        return NewLeaf;
    
    NewLeaf -> data   = elem;
    NewLeaf -> parent = nullptr;
    NewLeaf -> left   = nullptr;
    NewLead -> right  = nullptr;

    return NewLeaf;

}

bool Add_Unlinked_Leaf_Very_Very_Dangerous_Only_For_Developer_Of_This_Tree (const Element_t elem, const Tree_node* branch, const Tree* tree) {

    ASSERTTREE (tree);

    if (!ParentsCheck (branch, tree))
        return false;

    Tree_node* NewLeaf = NewNode (elem);

    if (NewLeaf == nullptr) {
        tree -> errcode = NoMemoryForNewNode;
        return false;
    
    tree -> size++;

    return true;

}

bool AddLeftLeaf (const Element_t elem, const Tree_node* branch, const Tree* tree) {

    ASSERTTREE (tree);

    if (!Add_Unlinked_Leaf_Very_Very_Dangerous_Only_For_Developer_Of_This_Tree (elem, branch, tree))
        return false;
    
    branch -> left = NewLeaf;
    NewLeaf -> parent = branch;

    return true;

}

bool AddRightLeaf (const Element_t data, const Tree_node* branch, const Tree* tree) {

    ASSERTTREE (tree);

    if (!Add_Unlinked_Leaf_Very_Very_Dangerous_Only_For_Developer_Of_This_Tree (elem, branch, tree))
        return false;
    
    branch -> right = NewLeaf;
    NewLeaf -> parent = branch;

    return true;

}

bool DeleteBranch (Tree_node* branch, const Tree* tree) {

    ASSERTTREE (tree);
    
    if (!ParentsCheck (branch, tree))
        return false;

    if (branch -> left != nullptr)
        DeleteBranch (branch -> left, tree);

    if (branch -> right != nullptr)
        DeleteBranch (branch -> right, tree);

    if (branch != tree -> root) {

        if (branch -> parent -> left == branch)
            branch -> parent -> left = nullptr;

        else

        if (branch -> parent -> right == branch)
            branch -> parent -> right = nullptr;

    }

    else tree -> root = nullptr;

    free (branch);

    tree -> size--;

    return true;

}

#endif
