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
const char[] DumpFile = "tree.dot";

enum Error {

    Allright             = 0,
    NoMemoryForNewNode   = 1,
    CantReadTreeFromFile = 2,
    NotEmptyHasNoRoot    = 3,
    InvalidConnection    = 4

};

struct Tree_node {

    Element_t data;
    Tree_node* parent;
    Tree_node* left;
    Tree_node* right;

};

struct Tree {

    Tree_node* root;
    int        size;
    Error      errcode;

};

void TreeConstruct                                                   (Tree* tree);
void TreeDestruct                                                    (Tree* tree);

bool ParentsCheck                     (const Tree_node* branch, const Tree* tree, const int deep = 0);

Tree_node* NewNode          (const Element_t elem);
Tree_node* AddNodeAndCheck  (Tree* tree,                    const Element_t elem);
bool AddRoot                (Tree* tree,                    const Element_t elem);
bool AddLeftLeaf            (Tree* tree, Tree_node* branch, const Element_t elem);
bool AddRightLeaf           (Tree* tree, Tree_node* branch, const Element_t elem);

bool DeleteBranch           (Tree* tree, Tree_node* branch);

//Tree_node* SearchElement (const Element_t elem,               const Tree* tree);

void PrintPreNode                 (FILE* file, const Tree* tree);
void PrintPreTree                 (FILE* file, const Tree* tree);
void ReadLeft                     (FILE* file,       Tree* tree, Tree_node* node);
void ReadRight                    (FILE* file,       Tree* tree, Tree_node* node); 
void ReadPreNode                  (FILE* file,       Tree* tree, Tree_node* node);
void ReadPreTree                  (FILE* file,       Tree* tree);
void Print (FILE* file, const char* item);
void Print (FILE* file, const int item);
void Print (FILE* file, const char item);
void Print (FILE* file, const double item);

bool CheckConnection                        (const Tree* tree, Tree_node* branch); 
bool TreeOk                                                    (const Tree* tree);
void TreeDump                                                  (const Tree* tree);
void DrawNode                                 (FILE* file, const Tree_node* node);
void DrawTree                                      (FILE* file, const Tree* tree);
void ErrDecode                                     (FILE* file, const Tree* tree);

void TreeConstruct (Tree* tree) {

    tree -> root    = nullptr;
    tree -> size    = 0;
    tree -> errcode = Allright;

    return;

}

void TreeDestruct (Tree* tree) {

    ASSERTTREE (tree);

    DeleteBranch (tree -> root, tree);

    tree -> size    = 0;
    tree -> errcode = Allright;

    return;

}

bool ParentsCheck (const Tree_node* branch, const Tree* tree, const int deep) {

    if (deep > tree -> size)
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
    NewLeaf -> right  = nullptr;

    return NewLeaf;

}

Tree_node* AddNodeAndCheck (Tree* tree, const Element_t elem) {

    ASSERTTREE (tree);

    Tree_node* NewLeaf = NewNode (&elem);

    if (NewLeaf == nullptr) {
        tree -> errcode = NoMemoryForNewNode;
        return nullptr;

    }

    return NewLeaf;

}

bool AddRoot (Tree* tree, const Element_t elem) {

    ASSERTTREE (tree);

    Tree_node* Root = nullptr;

    if ((Root = AddNodeAndCheck (elem, tree)) == nullptr)
        return false;

    tree -> size++;
    tree -> root = Root;

    return true;
 
}

bool AddLeftLeaf (Tree* tree, Tree_node* branch, const Element_t elem) {

    ASSERTTREE (tree);

    Tree_node* NewLeaf = nullptr;

    if ((NewLeaf = AddNodeAndCheck (elem, tree)) == nullptr)
        return false;
    
    tree -> size++;
    branch -> left = NewLeaf;
    NewLeaf -> parent = branch;

    return true;

}

bool AddRightLeaf (Tree* tree, Tree_node* branch, const Element_t elem) {

    ASSERTTREE (tree);

    Tree_node* NewLeaf = nullptr;

    if ((NewLeaf = AddNodeAndCheck (elem, tree)) == nullptr)
        return false;
    
    tree -> size++;
    branch -> right = NewLeaf;
    NewLeaf -> parent = branch;

    return true;

}

bool DeleteBranch (Tree* tree, Tree_node* branch) {

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
    
    free (branch -> data);
    free (branch);

    tree -> size--;

    return true;

}

void PrintPreNode (FILE* file, const Tree_node* node) {

    fprintf (file, "{\"");
    Print (file, node -> data);
    fprintf (file, "\"");`
    
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

void ReadLeft (FILE* file, Tree* tree, Tree_node* node) {
    
    assert (file);
    assert (node);
    ASSERTTREE (tree);

    if (fgetc (file) == '@')
        node -> left = nullptr;

    else {
 
        fseek (file, -1, SEEK_CUR);
        AddLeftLeaf (0, node, tree);
        ReadPreNode (file, node -> left, tree);
        ASSERTTREE (tree);

    }

    return;

}

void ReadRight (FILE* file, Tree* tree, Tree_node* node) {

    assert (file);
    assert (node);
    ASSERTTREE (tree);

    if (fgetc (file) == '@')
        node -> right = nullptr;

    else {
 
         fseek (file, -1, SEEK_CUR);

         if (fgetc (file) != '{') {

            tree -> errcode = CantReadTreeFromFile;
            return;

        }

        AddRightLeaf (0, node, tree);
        ReadPreNode (file, node -> right, tree);
        ASSERTTREE (tree);

    }

    return;

}

void ReadPreNode (FILE* file, Tree* tree, Tree_node* node) {

    char buf[MaxStrBufSize] = "";

    fscanf (file, "\"%m[^\"]", buf);
    node -> data = (Element_t*) calloc (strlen (buf), sizeof (char));
    strncpy (node -> data, buf, strlen (buf));

    char bracket = 0;

    if ((bracket = fgetc (file)) == '{') {

        ReadLeft  (file, node, tree);
        ReadRight (file, node, tree);

        bracket = fgetc (file);

    }

    if (bracket != '}') {

        tree -> errcode = CantReadTreeFromFile;
        return;

    }
    
    return;

}

void ReadPreTree (FILE* file, Tree* tree) {

    ASSERTTREE (tree);

    if (fgetc (file) != '{') {

        tree -> errcode = CantReadTreeFromFile;
        return;

    }

    AddRoot (0, tree);

    ReadPreNode (file, tree -> root, tree);

    return;

}

void Print (FILE* file, const char* item) {

    fprintf (file, "%s", item);
    return;

}

void Print (FILE* file, const int item) {

    fprintf (file, "%d", item);
    return;

}

void Print (FILE* file, const char item) {

    fprintf (file, "%c", item);
    return;

}

void Print (FILE* file, const double item) {

    fprintf (file, "%lg", item);
    return;

}


bool CheckConnection (const Tree* tree, Tree_node* branch) {

    assert (tree);
    assert (branch);

    if (!ParentsCheck (tree, branch))
        return false;

    if (branch -> left != nullptr)
        if (!CheckConnection (tree, branch -> left))
            return false;

    if (branch -> right != nullptr)
        if (!CheckConnection (tree, bracnh -> right))
            return false;

    return true;

}

bool TreeOk (const Tree* tree) {

    if (tree == nullptr)
        return false;

    if ((tree -> root == nullptr) && (tree -> size > 0)) {
        
        tree -> errcode = NotEmptyHasNoRoot;
        return false;

    }

    if (!CheckConnection (const Tree* tree, Tree_node* branch)) {

        tree -> errcode = InvalidConnection;
        return false;

    }

    if (lst -> errcode != Allright)
        return false;

    return true;

}

void TreeDump (const Tree* tree) {

    FILE* fileout = fopen (DumpFile, "r");

    if (tree == nullptr) {

        printf ("ERROR Tree is nullptr\n");
        return;

    }
        
    DrawTree (fileout, tree);

    return;

}

void DrawNode (FILE* file, const Tree_node* node) {

    if (node -> left != nullptr) {

        fprintf (file , "\t%x[label = \"%p | %s | {%p | %p}\"]\n", node -> left, node -> left, node -> left -> data, node -> left -> left, node -> left -> right);
        fprintf (file, "%x -> %x\n", node, node -> left);
        DrawNode (file, node -> left);

    }
    
    if (node -> left != nullptr) {

        fprintf (file , "\t%x[label = \"%p | %s | {%p | %p}\"]\n", node -> right, node -> right, node -> right -> data, node -> right -> left, node -> right -> right);
        fprintf (file, "%x -> %x\n", node, node -> right);
        DrawNode (file, node -> right);

    }
    
    return;

}

void DrawTree (FILE* file, const Tree* tree) {

    assert (file);
    assert (tree);

    fprintf (file, "digraph\n{\n");
    fprintf (file, "node[shape=record]\n"); 

    fprintf ("\tTree[label=\"%p\"]\n", tree);
    ErrDecode (file, tree);

    if (tree -> size > 0) {

        fprintf (file , "\t%x[label = \"%p | %s | {%p | %p}\"]\n", tree -> root, tree -> root, tree -> root -> data, tree -> root -> left, tree -> root -> right);

        DrawNode (file, tree -> root);

    }
  
    fprintf (file, "}\n");

    return; 

}

void ErrDecode (FILE* file, Tree* tree) {

    assert (file);
    assert (tree);

    switch (tree -> errcode) {

    case Allright: fprintf (file, "\tError[label=\"Allright\"]\n"); break;
    case NoMemoryForNewNode: fprintf (file, "\tError[label=\"Allright\"]\n"); break;
    case CantReadTreeFromFile:  fprintf (file, "\tError[label=\"Allright\"]\n"); break;
    case NotEmptyHasNoRoot:  fprintf (file, "\tError[label=\"Allright\"]\n"); break;
    case InvalidConnection: fprintf (file, "\tError[label=\"Allright\"]\n"); break;

    default: fprintf (file, "\tError[label=\"Unexpected error\"]\n"); break;

    }

}

#endif
