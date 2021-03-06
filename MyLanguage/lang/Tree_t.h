/*! This version of tree for differentator */


#ifndef __TREE_T__

    #define __TREE_T__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "enum_type.h"
#include "std_op.h"

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

typedef union {

    double Ndata;
    OpType Odata; 
    char* Sdata;
    DecType Ddata;

} Element_t;

const int MaxDeep = 2048;
const char EmptySymb = '$';
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
    Tone_t tone;

};

Tree_node* InvalidPtr = (Tree_node*) 0xFFFFf;

struct Tree {

    char name[MaxStrBufSize];
    Tree_node* root;
    int        size;
    Error      errcode;

};

void __TreeConstruct                                                 (Tree* tree, const char* Name);
void TreeDestruct                                                    (Tree* tree);

bool ParentsCheck                     (const Tree_node* branch, const Tree* tree, const int deep = 0);
bool LinkNodes                        (Tree_node* _parent, Tree_node* _left = nullptr, Tree_node* _right = nullptr);

Tree_node* NewNode (const double elem, NodeType Type = TNum, Tree_node* Left = nullptr, Tree_node* Right = nullptr);
Tree_node* NewNode (const OpType elem, NodeType Type = TOp, Tree_node* Left = nullptr, Tree_node* Right = nullptr);
Tree_node* NewNode (const char*  elem, NodeType Type = TId, Tree_node* Left = nullptr, Tree_node* Right = nullptr);
Tree_node* NewNode (const DecType elem, NodeType Type = TDec, Tree_node* Left = nullptr, Tree_node* Right = nullptr);
Tree_node* NewNode (const Tone_t elem, NodeType Type = TNum, Tree_node* Left = nullptr, Tree_node* Right = nullptr);

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
void PrintPreNode                 (FILE* file,       Tree_node* node);
void PrintPreTree                 (FILE* file,       Tree* tree);
void ReadLeft                     (FILE* file,       Tree* tree, Tree_node* node);
void ReadRight                    (FILE* file,       Tree* tree, Tree_node* node); 
void ReadPreNode                  (FILE* file,       Tree* tree, Tree_node* node);
void ReadPreTree                  (FILE* file,       Tree* tree); 

Element_t FuncDef (const char* elem);
bool CheckNumType                                   (Tree_node* node, const char* Sdata);
void DefineNode                                     (Tree_node* node);
void DefId                                          (Tree_node* node);
void DefDec                                         (Tree_node* node);
void DefOp                                          (Tree_node* node);
void DefNum                                         (Tree_node* node);
Tree_node* ReadInfixNode          (FILE* file,       Tree* tree, Tree_node* prenode);
void ReadInfixTree                (FILE* file,       Tree* tree);   
void PrintInfixNode               (FILE* file,       Tree_node* node);
void PrintInfixNode               (FILE* file,       Tree* tree);

void Print (FILE* file, const char* item);
void Print (FILE* file, const int item);
void Print (FILE* file, const char item);
void Print (FILE* file, const double item);
void Print (FILE* file, Tree_node* node);
void PrintNodeData (FILE* file, Tree_node* node);

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

    if (tree -> root != nullptr) {

        DeleteBranch (tree, tree -> root);
        tree -> root = nullptr;

    }

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

bool LinkNodes (Tree_node* _parent, Tree_node* _left, Tree_node* _right) {

    assert (_parent);

    if (_left != nullptr) {

        _parent -> left = _left;
        _left -> parent = _parent;

    }

    if (_right != nullptr) {
 
        _parent -> right = _right;
        _right -> parent = _parent;

    }

    return true;

}

Tree_node* NewNode (const Tone_t elem, NodeType Type, Tree_node* Left, Tree_node* Right) {

    Element_t buf = {};
    Tree_node* node = __NewNode (buf);

    if (node == nullptr)
        return nullptr;

    node -> type = Type;

    LinkNodes (node, Left, Right);

    node -> tone = elem;

    return node;

}

Tree_node* NewNode (const OpType elem, NodeType Type, Tree_node* Left, Tree_node* Right) {

    Element_t buf = {};
    buf.Odata = elem;

    Tree_node* node = __NewNode (buf);

    if (node == nullptr)
        return nullptr;

    node -> type = Type;

    LinkNodes (node, Left, Right);

    return node;

}

Tree_node* NewNode (const double elem, NodeType Type, Tree_node* Left, Tree_node* Right) {

    Element_t buf = {};
    buf.Ndata = elem;

    Tree_node* node = __NewNode (buf);

    if (node == nullptr)
        return nullptr;

    node -> type = Type;

    LinkNodes (node, Left, Right);

    return node;

}

Tree_node* NewNode (const char* elem, NodeType Type, Tree_node* Left, Tree_node* Right) {

    Element_t buf = {};
    sscanf (elem, "%ms", &(buf.Sdata));

    Tree_node* node = __NewNode (buf);

    if (node == nullptr)
        return nullptr;

    node -> type = Type;

    LinkNodes (node, Left, Right);

    return node;

}

Tree_node* NewNode (const DecType elem, NodeType Type, Tree_node* Left, Tree_node* Right) {

    Element_t buf = {};
    buf.Ddata = elem;

    Tree_node* node = __NewNode (buf);

    if (node == nullptr)
        return nullptr;

    node -> type = Type;

    LinkNodes (node, Left, Right);

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

    return OutBranch;
    
}

bool DeleteBranch (Tree* tree, Tree_node* branch) {

    ASSERTTREE (tree);
    if (branch <= InvalidPtr)
        return true;

    if (!ParentsCheck (branch, tree))
        return false;

    if (branch -> left != nullptr && branch -> left != branch) {

        DeleteBranch (tree, branch -> left);
        branch -> left = nullptr;

    }

    if (branch -> right != nullptr && branch -> right != branch) {

        DeleteBranch (tree, branch -> right);
        branch -> right = nullptr;

    }

    if (branch -> parent != nullptr && branch -> parent != branch) {

        if (branch -> parent -> left == branch)
            branch -> parent -> left = nullptr;

        else

        if (branch -> parent -> right == branch)
            branch -> parent -> right = nullptr;

    }   

    else tree -> root = nullptr;

    if (branch -> type == TId && branch -> data.Sdata != nullptr)
        free (branch -> data.Sdata);

    free (branch);

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

void PrintPreNode (FILE* file, Tree_node* node) {

    assert (file);
    assert (node);

    fprintf (file, "{");
    Print (file, node);

    if (node -> left != nullptr || node -> right != nullptr) {

        if (node -> left != nullptr)
            PrintPreNode (file, node -> left);
        else 
            fprintf (file, "%c", EmptySymb);

        if (node -> right != nullptr)
            PrintPreNode (file, node -> right);
        else 
            fprintf (file, "{%c}", EmptySymb);

    }

    fprintf (file, "}");
    
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
    if (fgetc (file) == EmptySymb) {

        node -> left = nullptr;
        if (fgetc (file) != '}') {
            tree -> errcode = CantReadTreeFromFile;
            return;
        }

    }

    else {
 
        fseek (file, -1, SEEK_CUR);
        AddLeftLeaf (tree, node, {});
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
    if (fgetc (file) == EmptySymb) {

        node -> right = nullptr;
        if (fgetc (file) != '}') {
            tree -> errcode = CantReadTreeFromFile;
            return;
        }

    }

    else {
 
         fseek (file, -1, SEEK_CUR);

    char c = 0; 

         if ((c = fgetc (file)) != '{') {

            tree -> errcode = CantReadTreeFromFile;
            return;

        }

        AddRightLeaf (tree, node, {});
        ReadPreNode (file, tree, node -> right);
        ASSERTTREE (tree);

    }

    return;

}

void ReadPreNode (FILE* file, Tree* tree, Tree_node* node) {

    assert (file);
    ASSERTTREE (tree);
    assert (node);

    SkipSpace (file);
    fscanf (file, "%m[^{}#$]", &(node -> data.Sdata));
    DefineNode (node);
    char bracket = 0;

    SkipSpace (file);
    if ((bracket = fgetc (file)) == '{') {

        ReadLeft  (file, tree, node);
        SkipSpace (file);
        ReadRight (file, tree, node);
        SkipSpace (file);
        bracket = fgetc (file);

    } else if (bracket == EmptyNode) {

        node -> left = nullptr;
        if ((bracket = fgetc (file)) == '{') {  

            ReadRight (file, tree, node);
            SkipSpace (file);
            bracket = fgetc (file);
           
        }

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

    AddRoot (tree, {});

    ReadPreNode (file, tree, tree -> root);

    return;

}

Element_t FuncDef (const char* elem) {

    Element_t ftype = {};

    for (int i  = 0; i < NumOfOp; i++)

        if (strncmp (elem, op[i], MaxIdName) == 0) {

            ftype.Odata = (OpType) i;
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

    }

    else
        isNum = false;

    return isNum;

}

void DefineNode (Tree_node* node) {

    assert (node);

    switch (*(node -> data.Sdata)) {

        case 'I': node -> type = TId; DefId (node); break;

        case 'D': node -> type = TDec; DefDec (node); break;

        case 'O': node -> type = TOp; DefOp  (node); break;

        case 'N': node -> type = TNum; DefNum (node); break;

        case 'C': node -> type = TCon; break;

    }
    
    return;

}

void DefId (Tree_node* node) {

    char* id = (char*) calloc (sizeof (char), MaxStrBufSize);
    
    sscanf (node -> data.Sdata + 2, "%s", id);

    assert (id);    

    free (node -> data.Sdata);

    if (strncmp (id, "main", 4) == 0)
        strncpy (id, "MainTheme", 9);

    node -> data.Sdata = id;

    return;

}

void DefDec (Tree_node* node) {

    char symb = *(node -> data.Sdata + 2);

    for (int i = 0; i < DecNum; i++)
    
        if (symb == *DecNames[i]) {
            
            free (node -> data.Sdata);
            node -> data.Ddata = (DecType) i;
            break;

        } 

    return;

}

void DefOp (Tree_node* node) {

    char* symb = node -> data.Sdata + 2;

    if (strncmp (node -> data.Sdata + 2, "C", 2) == 0) {

        free (node -> data.Sdata);
        node -> type = TCon;
        return;

    }

    for (int i = 0; i < NumOfOp; i++)
    
        if (strncmp (symb, std_op[i], strlen (symb) + 1) == 0) {
            
            free (node -> data.Sdata);
            node -> data.Odata = (OpType) i;
            break;

        } 

    return;

}

void DefNum (Tree_node* node) {

    double numBuf = atof (node -> data.Sdata + 2);

    free (node -> data.Sdata);
    node -> data.Ndata = numBuf;

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

    //DefineNodeType (node, DataBuf); 

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

    Print (file, node -> data.Odata);

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

void Print (FILE* file, Tree_node* node) {

    switch (node -> type) {

    case TOp: fprintf (file, "O:%s#", std_op[node -> data.Odata]); break;

    case TDec: fprintf (file, "O:%s#", DecNames[node -> data.Ddata]); break;

    case TId: { 

        if (strncmp (node -> data.Sdata, "MainTheme", 10) == 0)
            fprintf (file, "I:main#");

        else fprintf (file, "I:%s#", node -> data.Sdata); 

        break;

    }

    case TCon: fprintf (file, "O:C#"); break;

    default: fprintf (file, "N:%lg", node -> data.Ndata); break;

    }

    return;
   
}

void PrintNodeData (FILE* file, Tree_node* node) {

    switch (node -> type) {

    case TOp: fprintf (file, "%s", op[node -> data.Odata]); break;

    case TDec: fprintf (file, "%s", DecNames[node -> data.Ddata]); break;

    case TId: { 

        if (strncmp (node -> data.Sdata, "MainTheme", 10) == 0)
            fprintf (file, "main");

        else fprintf (file, "%s", node -> data.Sdata); 

        break;

    }

    default: fprintf (file, "%lg", node -> data.Ndata); break;

    }

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

/*    if (tree -> size > 0)

        if (!CheckConnection (tree, tree -> root)) {

            tree -> errcode = InvalidConnection;
            return false;

        }
*/

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

    system ("dot tree.dot -T png -o tree.png");

    return;

}

void DrawNode (FILE* file, const Tree_node* node) {

    if (node -> left != nullptr) {

        fprintf (file , "\tN%x[fillcolor=blue, label = \"{{%p} | {", node -> left, node -> left);

        PrintNodeData (file, node -> left);

        fprintf (file, " | %d | %d} | {<l> %p |<r> %p}}\"]\n", node -> left -> type, node -> left -> tone, node -> left -> left, node -> left -> right);
        fprintf (file, "<l> N%x -> N%x\n", node, node -> left);
        DrawNode (file, node -> left);

    }
    
    if (node -> right != nullptr) {

        fprintf (file , "\tN%x[fillcolor=green, label = \"{{%p} | {", node -> right, node -> right);
    
        PrintNodeData (file, node -> right);

        fprintf (file, " | %d | %d} | {<l> %p |<r> %p}}\"]\n", node -> right -> type, node -> right -> tone, node -> right -> left, node -> right -> right);
        fprintf (file, "<r> N%x -> N%x\n", node, node -> right);
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

        fprintf (file , "\tN%x[fillcolor=red, label = \"{{%p} | {", tree -> root, tree -> root);

        PrintNodeData (file, tree -> root);

        fprintf (file, " | %d | %d} | {<l> %p | <r> %p}}\"]\n", tree -> root -> type, tree -> root -> tone, tree -> root -> left, tree -> root -> right);

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

        fprintf (file , "\tN%x[fillcolor=blue, label = \"%d\"]\n", node -> left, node -> left -> data.Odata);
        fprintf (file, "N%x -> N%x\n", node, node -> left);
        DrawEasyNode (file, node -> left);

    }
    
    if (node -> right != nullptr) {

        fprintf (file , "\tN%x[fillcolor=green, label = \"%d\"]\n", node -> right, node -> right -> data.Odata);
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

        fprintf (file , "\tN%x[fillcolor=red, label = \"%d\"]\n", tree -> root, tree -> root, tree -> root -> data.Odata);
        DrawEasyNode (file, tree -> root);

    }
  
    fprintf (file, "}\n");

    return; 

}

#endif
