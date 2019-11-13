#ifndef __TREE_T__

    #define __TREE_T__

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

#define _DEBUG

#ifdef _DEBUG

    #define TreeDump(tree) __TreeDump (tree, __LINE__, __FUNCTION__)

    #define ASSERTTREE( cond )  \
        if (!TreeOk (cond)) {   \
            TreeDump (cond);    \
            assert (0); }       

#else

    #define assert( cond )

    #define ASSERTTREE( cond )

#endif

#define TreeConstruct(tree) __TreeConstruct(tree, #tree);

typedef char* Element_t;

const int MaxStrBufSize = 256;
const char DumpFile[] = "tree.dot";

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

    char name[MaxStrBufSize];
    Tree_node* root;
    int        size;
    Error      errcode;

};

void __TreeConstruct                                                 (Tree* tree, const char* Name);
void TreeDestruct                                                    (Tree* tree);

bool ParentsCheck                     (const Tree_node* branch, const Tree* tree, const int deep = 0);

Tree_node* NewNode          (const Element_t elem);
Tree_node* AddNodeAndCheck  (Tree* tree,                    const Element_t elem);
bool AddRoot                (Tree* tree,                    const Element_t elem);
bool AddLeftLeaf            (Tree* tree, Tree_node* branch, const Element_t elem);
bool AddRightLeaf           (Tree* tree, Tree_node* branch, const Element_t elem);

bool DeleteBranch           (Tree* tree, Tree_node* branch);

Tree_node* SearchElement      (Tree*      tree,   const char* elem);
Tree_node* SearchElemInBranch (Tree_node* branch, const char* elem);
 
void ScipSpace                    (FILE* file);
void PrintPreNode                 (FILE* file,       Tree_node* node, int deep = 0);
void PrintPreTree                 (FILE* file,       Tree* tree);
void ReadLeft                     (FILE* file,       Tree* tree, Tree_node* node);
void ReadRight                    (FILE* file,       Tree* tree, Tree_node* node); 
void ReadPreNode                  (FILE* file,       Tree* tree, Tree_node* node);
void ReadPreTree                  (FILE* file,       Tree* tree);
void Print (FILE* file, const char* item);
void Print (FILE* file, const int item);
void Print (FILE* file, const char item);
void Print (FILE* file, const double item);

bool CheckConnection                        (const Tree* tree, Tree_node* branch); 
bool TreeOk                                                          (Tree* tree);
void __TreeDump                                                (const Tree* tree, int line, const char* func);
void DrawNode                                      (FILE* file, const Tree_node* node);
void ErrDecode                                     (FILE* file, const Tree* tree);
void DrawTree                                      (FILE* file, const Tree* tree);
void DrawEasyNode                                  (FILE* file, const Tree_node* node);
void DrawEasyTree                                  (FILE* file, const Tree* tree);

void __TreeConstruct (Tree* tree, const char* Name) {

    assert (Name);

    strncpy (tree -> name, Name, MaxStrBufSize);
    tree -> root    = nullptr;
    tree -> size    = 0;
    tree -> errcode = Allright;

    return;

}

void TreeDestruct (Tree* tree) {

    ASSERTTREE (tree);

    DeleteBranch (tree, tree -> root);

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

    Tree_node* NewLeaf = NewNode (elem);

    if (NewLeaf == nullptr) {
        tree -> errcode = NoMemoryForNewNode;
        return nullptr;

    }

    return NewLeaf;

}

bool AddRoot (Tree* tree, const Element_t elem) {

    ASSERTTREE (tree);

    Tree_node* Root = nullptr;

    if ((Root = AddNodeAndCheck (tree, elem)) == nullptr)
        return false;

    tree -> size++;
    tree -> root = Root;

    return true;
 
}

bool AddLeftLeaf (Tree* tree, Tree_node* branch, const Element_t elem) {

    ASSERTTREE (tree);

    Tree_node* NewLeaf = nullptr;

    if ((NewLeaf = AddNodeAndCheck (tree, elem)) == nullptr)
        return false;
    
    tree -> size++;
    branch -> left = NewLeaf;
    NewLeaf -> parent = branch;

    return true;

}

bool AddRightLeaf (Tree* tree, Tree_node* branch, const Element_t elem) {

    ASSERTTREE (tree);

    Tree_node* NewLeaf = nullptr;

    if ((NewLeaf = AddNodeAndCheck (tree, elem)) == nullptr)
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
        DeleteBranch (tree, branch -> left);

    if (branch -> right != nullptr)
        DeleteBranch (tree, branch -> right);

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

//should be Overload function!!!!

Tree_node* SearchElement (Tree* tree, const char* elem) {

    ASSERTTREE (tree);
    assert (elem);

    if (tree -> root == nullptr)
        return nullptr;

    return SearchElemInBranch (tree -> root, elem);

}

Tree_node* SearchElemInBranch (Tree_node* branch, const char* elem) {

    assert (branch);
    assert (elem);

    if (strncmp (elem, branch -> data, MaxStrBufSize) == 0)
        return branch;

    Tree_node* place = nullptr;

    if (branch -> left != nullptr)
        if ((place = SearchElemInBranch (branch -> left, elem)) != nullptr)
            return place;

    if (branch -> right != nullptr)
        if ((place = SearchElemInBranch (branch -> right, elem)) != nullptr)
            return place;

    return place;

}

void SkipSpace (FILE* file) {
    
    assert (file);
    fscanf (file, "%*[\n\t\r]");

    return;

}

void PrintPreNode (FILE* file, Tree_node* node, int deep) {

    assert (file);
    assert (node);
    assert (deep >= 0);

    for (int i = 0; i < deep; ++i)
        fprintf (file, "\t");
 
    deep++;
    fprintf (file, "{\"");
    Print (file, node -> data);
    fprintf (file, "\"");

   
    if (node -> left != nullptr || node -> right != nullptr) {

        fprintf (file, "\n");

        if (node -> left != nullptr)
            PrintPreNode (file, node -> left, deep);
        else 
            fprintf (file, "@");

        if (node -> right != nullptr)
            PrintPreNode (file, node -> right, deep);
        else 
            fprintf (file, "@");

        for (int i = 0; i < deep - 1; ++i)
            fprintf (file, "\t");

    }

    fprintf (file, "}\n");
    
    return;

}

void PrintPreTree (FILE* file, Tree* tree) {

    ASSERTTREE (tree);
    assert (file);

    PrintPreNode (file, tree -> root);

    return;

}

void ReadLeft (FILE* file, Tree* tree, Tree_node* node) {
    
    assert (file);
    assert (node);
    ASSERTTREE (tree);
    
    SkipSpace (file);
    if (fgetc (file) == '@')
        node -> left = nullptr;

    else {
 
        fseek (file, -1, SEEK_CUR);
        AddLeftLeaf (tree, node, 0);
        ReadPreNode (file, tree, node -> left);
        ASSERTTREE (tree);

    }

    return;

}

void ReadRight (FILE* file, Tree* tree, Tree_node* node) {

    assert (file);
    assert (node);
    ASSERTTREE (tree);

    SkipSpace (file);
    if (fgetc (file) == '@')
        node -> right = nullptr;

    else {
 
         fseek (file, -1, SEEK_CUR);

         if (fgetc (file) != '{') {

            tree -> errcode = CantReadTreeFromFile;
            return;

        }

        AddRightLeaf (tree, node, 0);
        ReadPreNode (file, tree, node -> right);
        ASSERTTREE (tree);

    }

    return;

}

void ReadPreNode (FILE* file, Tree* tree, Tree_node* node) {

    SkipSpace (file);
    fscanf (file, "\"%m[^\"]\"", &(node -> data));
    char bracket = 0;

    SkipSpace (file);
    if ((bracket = fgetc (file)) == '{') {

        ReadLeft  (file, tree, node);
        ReadRight (file, tree, node);
        SkipSpace (file);
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
    assert (file);

    SkipSpace (file);
    if (fgetc (file) != '{') {

        tree -> errcode = CantReadTreeFromFile;
        return;

    }

    AddRoot (tree, 0);

    ReadPreNode (file, tree, tree -> root);

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

    if (!ParentsCheck (branch, tree))
        return false;

    if (branch -> left != nullptr)
        if (!CheckConnection (tree, branch -> left))
            return false;

    if (branch -> right != nullptr)
        if (!CheckConnection (tree, branch -> right))
            return false;

    return true;

}

bool TreeOk (Tree* tree) {

    if (tree == nullptr)
        return false;

    if ((tree -> root == nullptr) && (tree -> size > 0)) {
        
        tree -> errcode = NotEmptyHasNoRoot;
        return false;

    }

    if (tree -> size > 0)

        if (!CheckConnection (tree, tree -> root)) {

            tree -> errcode = InvalidConnection;
            return false;

        }

    if (tree -> errcode != Allright)
        return false;

    return true;

}

void __TreeDump (const Tree* tree, int line, const char* func) {

    FILE* fileout = fopen (DumpFile, "w");

    assert (fileout);

    fprintf (fileout, "digraph\n{\n");
    fprintf (fileout, "node[shape=record]\n"); 
 
    if (tree == nullptr) {

        fprintf (fileout, "\tERROR [label=\"ERROR Tree is nullptr\"]\n");
        fclose (fileout);
        return;

    }

    fprintf (fileout, "\tline [label=\"Line %d\"]\n", line);
    fprintf (fileout, "\tfunc [label=\"In function %s\"]\n", func);
        
    DrawTree (fileout, tree);

    fclose (fileout);

    return;

}

void DrawNode (FILE* file, const Tree_node* node) {

    if (node -> left != nullptr) {

        fprintf (file , "\t%x[fillcolor=blue, label = \"{%p | %s | {%p |%p}}\"]\n", node -> left, node -> left, node -> left -> data, node -> left -> left, node -> left -> right);
        fprintf (file, "%x -> %x\n", node, node -> left);
        DrawNode (file, node -> left);

    }
    
    if (node -> right != nullptr) {

        fprintf (file , "\t%x[fillcolor=green, label = \"{%p | %s | {%p |%p}}\"]\n", node -> right, node -> right, node -> right -> data, node -> right -> left, node -> right -> right);
        fprintf (file, "%x -> %x\n", node, node -> right);
        DrawNode (file, node -> right);

    }
    
    return;

}

void DrawTree (FILE* file, const Tree* tree) {

    assert (file);
    assert (tree);

    fprintf (file, "\tTree[label=\"%p | %s\"]\n", tree, tree->name);
    ErrDecode (file, tree);

    fprintf (file, "node[style=filled]\n");

    if (tree -> size > 0) {

        fprintf (file , "\t%x[fillcolor=red, label = \"{%p | %s | {%p |%p}}\"]\n", tree -> root, tree -> root, tree -> root -> data, tree -> root -> left, tree -> root -> right);

        DrawNode (file, tree -> root);

    }
  
    fprintf (file, "}\n");

    return; 

}

void ErrDecode (FILE* file, const Tree* tree) {

    assert (file);
    assert (tree);

    switch (tree -> errcode) {

    case Allright: fprintf (file, "\tError[label=\"Allright\"]\n"); break;
    case NoMemoryForNewNode: fprintf (file, "\tError[label=\"No Memory For New Node\"]\n"); break;
    case CantReadTreeFromFile:  fprintf (file, "\tError[label=\"Cannot Read Tree From File\"]\n"); break;
    case NotEmptyHasNoRoot:  fprintf (file, "\tError[label=\"Not Empty tree but root is null\"]\n"); break;
    case InvalidConnection: fprintf (file, "\tError[label=\"Invalid Connection\"]\n"); break;

    default: fprintf (file, "\tError[label=\"Unexpected error\"]\n"); break;

    }

    return;

}

void DrawEasyNode (FILE* file, const Tree_node* node) {

    assert (file);
    assert (node);

    if (node -> left != nullptr) {

        fprintf (file , "\t%x[fillcolor=blue, label = \"%s\"]\n", node -> left, node -> left -> data);
        fprintf (file, "%x -> %x\n", node, node -> left);
        DrawEasyNode (file, node -> left);

    }
    
    if (node -> right != nullptr) {

        fprintf (file , "\t%x[fillcolor=green, label = \"%s\"]\n", node -> right, node -> right -> data);
        fprintf (file, "%x -> %x\n", node, node -> right);
        DrawEasyNode (file, node -> right);

    }

    return;

}

void DrawEasyTree (FILE* file, const Tree* tree) {

    assert (file);
    assert (tree);

    fprintf (file, "digraph\n{\n");
    fprintf (file, "node[shape=record]\n"); 
 
    fprintf (file, "node[style=filled]\n");

    if (tree -> size > 0) {

        fprintf (file , "\t%x[fillcolor=red, label = \"%s\"]\n", tree -> root, tree -> root -> data);
        DrawEasyNode (file, tree -> root);

    }
  
    fprintf (file, "}\n");

    return; 

}

#endif
