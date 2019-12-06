/*! This version of tree for differentator */


#ifndef __TREE_T__

    #include "enum_type.h"

    #define __TREE_T__

#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "enum_type.h"

#define _DEBUG

#ifdef _DEBUG

    #define TreeDump(tree) __TreeDump (tree, __LINE__, __FUNCTION__)

    #define ASSERTTREE( cond )  \
        if (!TreeOk (cond)) {   \
            TreeDump (cond);    \
            assert (TreeErroR); }       

#else

    #define assert( cond )
    #define ASSERTTREE( cond )

#endif

#define TreeConstruct(tree) __TreeConstruct(tree, #tree);

const int TreeErroR = 0;
const int MaxDeep = 1000;

typedef union {

    double Ndata;
    FuncType Fdata;

} Element_t;

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

    Element_t  data;
    Tree_node* parent;
    Tree_node* left;
    Tree_node* right;
    NodeType type;
    int priority;

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

Tree_node* NewNode (const double elem, NodeType type, Tree_node* Left = nullptr, Tree_node* Right = nullptr);
Tree_node* NewNode (const FuncType elem, NodeType type, Tree_node* Left = nullptr, Tree_node* Right = nullptr);

Tree_node* __NewNode        (const Element_t elem);
Tree_node* AddNodeAndCheck  (Tree* tree,                    const Element_t elem);
bool AddRoot                (Tree* tree,                    const Element_t elem);
bool AddLeftLeaf            (Tree* tree, Tree_node* branch, const Element_t elem);
bool AddRightLeaf           (Tree* tree, Tree_node* branch, const Element_t elem);

void CopyBranch (Tree_node* SourceBranch, Tree_node* OutBranch, Tree* OutTree);
Tree_node* CopyOfBranch (Tree_node* SourceBranch);

bool DeleteBranch           (Tree* tree, Tree_node* branch);
void CutBrunchWithoutLeaf   (Tree* tree, Tree_node* branch, Tree_node* leaf);

void SkipSpace                    (FILE* file);

Element_t FuncDef (const char* elem);
bool CheckNumType                                   (Tree_node* node, const char* Sdata);
void DefineNodeType                                 (Tree_node* node, const char* Sdata);
Tree_node* ReadInfixNode          (FILE* file,       Tree* tree, Tree_node* prenode);
void ReadInfixTree                (FILE* file,       Tree* tree);   
void PrintInfixNode               (FILE* file,       Tree_node* node);
void PrintInfixNode               (FILE* file,       Tree* tree);

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

    if (deep > MaxDeep)
        return false;

    if (branch == tree -> root)
        return true;

    if (branch -> parent == nullptr)
        return false;

    return ParentsCheck (branch -> parent, tree, deep + 1);

}

Tree_node* NewNode (const FuncType elem, NodeType Type, Tree_node* Left, Tree_node* Right) {

    Element_t buf = {};
    buf.Fdata = elem;

    Tree_node* node = __NewNode (buf);

    if (node == nullptr)
        return nullptr;

    int prior = 0;

    if (Type == TFunc) {

        #define NODE_TYPE(name, signature, num, priority, code) \
            if (elem == type_##name) \
                prior = priority;

        #include "types.h"

        #undef NODE_TYPE

    }

    node -> type = Type;
    node -> priority = prior;

    node -> left = Left;
    node -> right = Right;
    
    if (Left != nullptr)
        Left -> parent = node;

    if (Right != nullptr)
        Right -> parent = node;

    return node;

}

Tree_node* NewNode (const double elem, NodeType Type, Tree_node* Left, Tree_node* Right) {

    Element_t buf = {};
    buf.Ndata = elem;

    Tree_node* node = __NewNode (buf);

    if (node == nullptr)
        return nullptr;

    node -> type = Type;
    node -> priority = 0;

    node -> left = Left;
    node -> right = Right;
    
    if (Left != nullptr)
        Left -> parent = node;

    if (Right != nullptr)
        Right -> parent = node;

    return node;

}

Tree_node* __NewNode (const Element_t elem) {

    Tree_node* NewLeaf = (Tree_node*) calloc (1, sizeof (Tree_node));

    if (NewLeaf == nullptr)
        return NewLeaf;
    
    NewLeaf -> data              = elem;
    NewLeaf -> parent            = nullptr;
    NewLeaf -> left              = nullptr;
    NewLeaf -> right             = nullptr;

    return NewLeaf;

}

Tree_node* AddNodeAndCheck (Tree* tree, const Element_t elem) {

    assert (tree);

    Tree_node* NewLeaf = __NewNode (elem);

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

void CopyBranch (Tree_node* SourceBranch, Tree_node* OutBranch, Tree* OutTree) {

    ASSERTTREE (OutTree);
    assert (SourceBranch);
    assert (OutBranch);

    if (SourceBranch -> left != nullptr) {
        AddLeftLeaf (OutTree, OutBranch, {});
        CopyBranch (SourceBranch -> left, OutBranch -> left, OutTree);
    }

    if (SourceBranch -> right != nullptr) {
        AddRightLeaf (OutTree, OutBranch, {});
        CopyBranch (SourceBranch -> right, OutBranch -> right, OutTree);
    }

    OutBranch -> data = SourceBranch -> data;
    OutBranch -> type = SourceBranch -> type;
    OutBranch -> priority = SourceBranch -> priority;

    return;
    
}

Tree_node* CopyOfBranch (Tree_node* SourceBranch) {

    assert (SourceBranch);

    Tree_node* OutBranch = NewNode (0, {});

    if (SourceBranch -> left != nullptr) 
        OutBranch -> left = CopyOfBranch (SourceBranch -> left);

    if (SourceBranch -> right != nullptr)
        OutBranch -> right = CopyOfBranch (SourceBranch -> right);

    OutBranch -> data = SourceBranch -> data;
    OutBranch -> type = SourceBranch -> type;
    OutBranch -> priority = SourceBranch -> priority;

    return OutBranch;
    
}

bool DeleteBranch (Tree* tree, Tree_node* branch) {

    ASSERTTREE (tree);
   
    if (!ParentsCheck (branch, tree))
        return false; 

    if (branch -> left != nullptr)
        DeleteBranch (tree, branch -> left);

    if (branch -> right != nullptr)
        DeleteBranch (tree, branch -> right);

    if (branch -> parent != nullptr) {

        if (branch -> parent -> left == branch)
            branch -> parent -> left = nullptr;

        else

        if (branch -> parent -> right == branch)
            branch -> parent -> right = nullptr;

    }   

    else tree -> root = nullptr;
    
    free (branch);

//    tree -> size--;

    return true;

}

void CutBrunchWithoutLeaf (Tree* tree, Tree_node* branch, Tree_node* leaf) {

    ASSERTTREE (tree);

    ParentsCheck (branch, tree);
    ParentsCheck (leaf, tree);
    
    ASSERTTREE (tree);

    Tree_node* TmpNodePtr = leaf;

    Tree_node* Parent = branch -> parent;
    
    if (leaf == branch -> left)
        branch -> left = nullptr;

    else if (leaf == branch -> right)
        branch -> right = nullptr;
   
    if (Parent != nullptr) {

        if (branch -> parent -> left == branch) {

            DeleteBranch (tree, branch);
            Parent -> left = TmpNodePtr;
               
        }

        else if (branch -> parent -> right == branch) {

           DeleteBranch (tree, branch);
           Parent -> right = TmpNodePtr;
 
        }

    }

    else {

        DeleteBranch (tree, branch);
        tree -> root = TmpNodePtr;

    }

    TmpNodePtr -> parent = Parent;

    return;

}

void SkipSpace (FILE* file) {
    
    assert (file);
    fscanf (file, "%*[ \n\t\r]");

    return;

}

Element_t FuncDef (const char* elem) {

    Element_t ftype = {};

    for (int i  = 0; i < NumOfFunc; i++)

        if (strncmp (elem, func[i], MaxIdName) == 0) {

            ftype.Fdata = (FuncType) i;
            break;
    
        }

    return ftype;   

}

bool CheckNumType (Tree_node* node, const char* Sdata) {

    assert (node);

    bool isNum = true;
    double NumCheckBuf = 0;

    if (sscanf (Sdata, "%lg", &NumCheckBuf) > 0) {

        node -> data.Ndata = atof (Sdata);
        node -> type = TNum;
        node -> priority = 0;

    }

    else
        isNum = false;

    return isNum;

}

void DefineNodeType (Tree_node* node, const char* Sdata) {

    assert (node);

    #define NODE_TYPE(name, signature, num, prior, code)              \
            if (type_##name == type_NoType);                          \
            else  if (type_##name == type_Num)                        \
                CheckNumType (node, Sdata);                           \
            else                                                      \
            if (strncmp (Sdata, signature, MaxComLength) == 0){       \
                node -> type = TFunc;                                 \
                node -> priority = prior; }

    #include "types.h" ;


    #undef NODE_TYPE

    return;

}

Tree_node* ReadInfixNode (FILE* file, Tree* tree, Tree_node* prenode) {

    assert (tree);
    assert (file);

    Tree_node* node = AddNodeAndCheck (tree, {});

    char bracket = 0;

    SkipSpace (file);
    if ((bracket = fgetc (file)) == '(') {

        node -> left = ReadInfixNode  (file, tree, node);
        SkipSpace (file);

    }

    else fseek (file, -1, SEEK_CUR);

    char* DataBuf = nullptr;

    fscanf (file, "%m[^ ()]", &DataBuf);

    DefineNodeType (node, DataBuf); 

    SkipSpace (file);

    if ((bracket = fgetc (file)) == '(') {
        
        node -> right = ReadInfixNode (file, tree, node);
        SkipSpace (file);

        bracket = fgetc (file);

    }


    if (bracket != ')') {

        tree -> errcode = CantReadTreeFromFile;
        return nullptr;

    }
    
    tree -> size++;
    
    node -> parent = prenode;

    return node;   

}

void ReadInfixTree (FILE* file, Tree* tree) {

    ASSERTTREE (tree);
    assert (file);

    SkipSpace (file);
    if (fgetc (file) != '(') {

        tree -> errcode = CantReadTreeFromFile;
        return;

    }

    tree -> root = ReadInfixNode (file, tree, tree -> root);

    return;

}

void PrintInfixNode (FILE* file, Tree_node* node) {

    assert (file);

    if (node == nullptr)
        return;

    fprintf (file, "(");

    PrintInfixNode (file, node -> left);

    Print (file, node -> data.Fdata);

    PrintInfixNode (file, node -> right);

    fprintf (file, ")");

    return;

}

void PrintInfixTree (FILE* file, Tree* tree) {

    ASSERTTREE (tree);
    assert (file);

    PrintInfixNode (file, tree -> root);    

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

    if (tree -> root != nullptr)

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

        fprintf (fileout, "\tERROR [label=\"ERROR Tree is nullptr\"]\n}");
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

        fprintf (file , "\tN%x[fillcolor=blue, label = \"{{%p | %d} | {%d | %d} | {%p |%p}}\"]\n", node -> left, node -> left, node -> left -> priority, node -> left -> data.Fdata, node -> left -> type, node -> left -> left, node -> left -> right);
        fprintf (file, "N%x -> N%x\n", node, node -> left);
        DrawNode (file, node -> left);

    }
    
    if (node -> right != nullptr) {

        fprintf (file , "\tN%x[fillcolor=green, label = \"{{%p | %d} | {%d | %d} | {%p |%p}}\"]\n", node -> right, node -> right, node -> right -> priority, node -> right -> data.Fdata, node -> right -> type, node -> right -> left, node -> right -> right);
        fprintf (file, "N%x -> N%x\n", node, node -> right);
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

    if (tree -> root != nullptr) {

        fprintf (file , "\tN%x[fillcolor=red, label = \"{{%p | %d} | {%d | %d} | {%p |%p}}\"]\n", tree -> root, tree -> root, tree -> root -> priority, tree -> root -> data.Fdata, tree -> root -> type, tree -> root -> left, tree -> root -> right);

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

        fprintf (file , "\tN%x[fillcolor=blue, label = \"%d\"]\n", node -> left, node -> left -> data.Fdata);
        fprintf (file, "N%x -> N%x\n", node, node -> left);
        DrawEasyNode (file, node -> left);

    }
    
    if (node -> right != nullptr) {

        fprintf (file , "\tN%x[fillcolor=green, label = \"%d\"]\n", node -> right, node -> right -> data.Fdata);
        fprintf (file, "N%x -> N%x\n", node, node -> right);
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

        fprintf (file , "\tN%x[fillcolor=red, label = \"%d\"]\n", tree -> root, tree -> root -> data.Fdata);
        DrawEasyNode (file, tree -> root);

    }
  
    fprintf (file, "}\n");

    return; 

}

#endif
