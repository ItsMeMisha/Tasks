#ifndef __TREE_T__

    #define __TREE_T__

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

#ifdef _DEBUG

    #define assert( cond ) assert (cond)

    #define ASSERTTREE( cond )  \
        if (!TreeOk (cond)) {   \
            TreeDump (cond);    \
            assert (0); }       

#else

    #define assert( cond )

    #define ASSERTTREE( cond )

#endif

typedef char* Element_t;

const int MaxStrBufSize = 512;

enum Error {

    Allright           = 0;
    NoMemoryForNewNode = 1;

};

struct Tree_node {

    Element_t* data;
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

//Tree_node* SearchElement (const Element_t elem,                          const Tree* tree);

void PrintPreNode                                           (FILE* file, const Tree* tree);
void PrintPreTree                                           (FILE* file, const Tree* tree);
void ReadPreNode                                    (const char* StrBuf, const Tree* tree);
void ReadPreTree                                            (FILE* file, const Tree* tree);
void Print (FILE* file, const char* item);
void Print (FILE* file, const int item);
void Print (FILE* file, const char item);
void Print (FILE* file, const double item);

bool TreeOk                                                             (const Tree* tree);
void TreeDump                                                           (const Tree* tree);
void DrawTree                                               (FILE* file, const Tree* tree);

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

Tree_node* NewNode (const Element_t* elem) {

    Tree_node* NewLeaf = (Tree_node*) calloc (1, sizeof (Tree_node));

    if (NewLeaf == nullptr)
        return NewLeaf;
    
    NewLeaf -> data = (Element_t*) calloc (1, sizeof (*elem));
    memncpy (NewLeaf -> data, elem, sizeof (*elem));
    NewLeaf -> parent = nullptr;
    NewLeaf -> left   = nullptr;
    NewLead -> right  = nullptr;

    return NewLeaf;

}

bool Add_Unlinked_Leaf_Very_Very_Dangerous_Only_For_Developer_Of_This_Tree (const Element_t elem, const Tree_node* branch, const Tree* tree) {

    ASSERTTREE (tree);

    if (!ParentsCheck (branch, tree))
        return false;

    Tree_node* NewLeaf = NewNode (&elem);

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

void PrintPreNode (FILE* file, const Tree_node* node) {

    fprintf (file, "{");
    Print (file, node -> data);
    
    if (node -> left != nullptr || node -> right != nullptr) {

        if (node -> left != nullptr)
            PrintPreNode (file, node -> left);
        else 
            fprintf (file, "@");

        if (node -> right != nullptr)
            PrintPreNode (file, node -> right);
        else 
            fprintf (file, "@");

    }

    fprintf (file, "}");
    
    return;

}

void PrintPreTree (FILE* file, const Tree* tree) {

    PrintPreNode (file, tree -> root);

    return;

}

void ReadPreNode (const char* StrBuf, const Tree_node* node) {

    return;

}

void ReadPreTree (FILE* file, const Tree* tree) {

    char StrBuf[MaxStrBufSize] = "";

    fscanf (file, "%512s", StrBuf);

    ReadPreNode (const char* StrBuf, tree -> root);

    return;

}

void Print (FILE* file, const char* item) {

    fprintf (file, "%s", item);

}

void Print (FILE* file, const int item) {

    fprintf (file, "%d", item);

}

void Print (FILE* file, const char item) {

    fprintf (file, "%c", item);

}

void Print (FILE* file, const double item) {

    fprintf (file, "%lg", item);

}


bool TreeOk                                                             (const Tree* tree);
void TreeDump                                                           (const Tree* tree);
void DrawTree                                               (FILE* file, const Tree* tree);

#endif
