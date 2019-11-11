#include "Tree_t.h"
#include <stdio.h>

#define _DEBUG

const int MaxStrSize = 256;
const int MaxFileNameSize = 128;
const char DataBaseFile[MaxFileNameSize] = "AkinatorData.txt";

bool YesOrNoRead ();

Tree* CreateQuestionTreeAndOpenDataBaseFile (Tree* tree, FILE** file);
bool Game (Tree* tree);
bool ReplaceData (Element_t* elem, Tree_node* node);
bool AddNewQuestion (Tree_node* question, Tree* tree);
void LastQuestion (Tree_node* question, Tree* tree);
void Ask (Tree_node* question, Tree* tree);

void PrintNewDataBaseAndCloseDataBaseFile (Tree* tree, FILE** file);

int main () {

    Tree tree = {};

    FILE* file = nullptr;
    if (CreateQuestionTreeAndOpenDataBaseFile (&tree, &file) == nullptr) {

        printf ("Database file error: cannot open file %s\n", DataBaseFile);
        return 1;

    }

    while (Game (&tree));

    PrintNewDataBaseAndCloseDataBaseFile (&tree, &file);

    TreeDestruct (&tree);

    return 0;

}

bool YesOrNoRead () {

    char answer[4] = "";

    scanf ("%s", answer);

    if (strncmp (answer, "Yes", 4) == 0 || strncmp (answer, "yes", 4) == 0)
        return true;
    else 
        return false;

}

Tree* CreateQuestionTreeAndOpenDataBaseFile (Tree* tree, FILE** file) {

    *file = fopen (DataBaseFile, "r");

    if (*file == nullptr)
        return nullptr;

    TreeConstruct (tree);
    ReadPreTree (*file, tree); 

    return tree;

}

bool Game (Tree* tree) {

    bool Continue = false;

    Ask (tree -> root, tree);

    printf ("Want to play one more time?\n");

    if (YesOrNoRead ())
        Continue = true; 

    return Continue;

}

bool ReplaceData (Element_t* elem, Tree_node* node) {

    assert (elem);
    assert (node);

    node -> data = (Element_t*) realloc (node -> data, sizeof (*elem));
    
    if (node -> data == nullptr)
        return false;

    memcpy (node -> data, elem, sizeof (*elem));

    return true;

}

bool AddNewQuestion (Tree_node* question, Tree* tree) {

    assert (question);

    char AnsBuf[MaxStrBufSize] = "";
    scanf ("%s", AnsBuf);

    printf ("\nAnd what the difference between %s and %s?\n", *(question -> data), AnsBuf);
    char QueBuf[MaxStrBufSize] = "";
    scanf ("%s", QueBuf);

    if (!AddLeftLeaf (AnsBuf, question, tree))
        return false;

    if (!AddRightLeaf (*(question -> data), question, tree))
        return false;

    if (!ReplaceData (&QueBuf, question))
        return false;

    printf ("%s %s added\n", QueBuf, AnsBuf);

    return true;

}

void LastQuestion (Tree_node* question, Tree* tree) {

    assert (question);
   
    if (YesOrNoRead ())
        printf ("I knew it!\n");

    else {

        printf ("Unbelievable! Who is it then?\n");
        AddNewQuestion (question, tree);

    }        

    return;
    
}

void Ask (Tree_node* question, Tree* tree) {

    printf ("%s?\n", *(question -> data));

    if (question -> left == nullptr && question -> right == nullptr)
        LastQuestion (question, tree);

    else

        if (question -> left == nullptr || question -> right == nullptr) {
            printf ("Database error: incomplete tree\n");
            return;
        }
    
        if (YesOrNoRead ())
            Ask (question -> left, tree);
        else
            Ask (question -> right, tree);

    return;

}

void PrintNewDataBaseAndCloseDataBaseFile (Tree* tree, FILE** file) {
    
    ASSERTTREE (tree);

    PrintPreTree (*file, tree);

    fclose (*file); 

    return;    

}

