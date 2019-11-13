#include <stdio.h>
#include <ctype.h>
#include "Tree_t.h"

#define _DEBUG

const int ShortAnsLen = 4;
const int MaxStrSize = 256;
const int MaxFileNameSize = 128;
const char DataBaseFile[MaxFileNameSize] = "AkinatorData.txt";

void StrToLower (char* str);
bool YesOrNoRead ();

Tree* CreateQuestionTreeAndOpenDataBaseFile (Tree* tree, FILE** file);

//void CompareMode (Tree* tree);
void ElemDef (Tree* tree, Tree_node* node);
void DefinMode (Tree* tree);
void ChooseGamemode (Tree* tree);
bool Game (Tree* tree);
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

    printf ("If you desturb me one more time, I will find you\n\n");
    while (Game (&tree));
    printf ("Goodbye, humanoid\n");    

    PrintNewDataBaseAndCloseDataBaseFile (&tree, &file);

    TreeDump (&tree);

    TreeDestruct (&tree);

    return 0;

}

void StrToLower (char* str) {

    assert (str);

    for (int i = 0; str[i] != '\0'; ++i)
        if (isalpha (str[i]))
            str[i] = tolower (str[i]);

    return;

}

bool YesOrNoRead () {

    char* answer = nullptr;
    scanf ("%ms", &answer);
    StrToLower (answer);

    if (strncmp (answer, "yes", ShortAnsLen) == 0)
        return true;
    else if (strncmp (answer, "no", ShortAnsLen) == 0)
        return false;
        
    printf ("What the hell have you typed?! Type normally 'yes' or 'no'!!!\n");
    return YesOrNoRead ();

}

Tree* CreateQuestionTreeAndOpenDataBaseFile (Tree* tree, FILE** file) {

    ASSERTTREE (tree);

    *file = fopen (DataBaseFile, "r");

    if (*file == nullptr)
        return nullptr;

    TreeConstruct (tree);
    ReadPreTree (*file, tree); 

    return tree;

}

//void CompareMode (Tree* tree);
void ElemDef (Tree* tree, Tree_node* node) {

    ASSERTTREE (tree);
    assert (node);

    if (!ParentsCheck (node, tree))
        return;

    if (node -> parent != tree -> root)
        ElemDef (tree, node -> parent);

    if (node == node -> parent -> left)
        printf ("\t%s \n", node -> parent -> data);
    
    if (node == node -> parent -> right)
        printf ("\tNot %s \n", node -> parent -> data);

    return;

}

void DefinMode (Tree* tree) {

    ASSERTTREE (tree);

    printf ("Ok, what do you want to know?\n");

    char* AnsBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* leaf = nullptr;

    if ((leaf = SearchElement (tree, AnsBuf)) != nullptr)
        ElemDef (tree, leaf);

    else
        printf ("You've invented this word, you are cheating, please turn me off, I don't wanna talk with you anymore \n");

}

void ChooseGamemode (Tree* tree) {

    ASSERTTREE (tree);
    
    printf ("Come on, choose gamemode: \n\n\t Guessing \n\t Definitions \n\t Comparing  (Coming soon! Make a pre-order only for 300$ and play it erlier!)\n\n");

    char* answer = nullptr;
    scanf ("%ms", &answer);
    StrToLower (answer);

    if (strncmp (answer, "guessing", MaxStrSize) == 0) {

        Ask (tree -> root, tree);
        return;
    }

    else if (strncmp (answer, "definitions", MaxStrSize) == 0) {

        DefinMode (tree);
        return;
    }

    printf ("What the hell have you typed?! Type normally!!!\n");
    ChooseGamemode (tree);
    return;

}

bool Game (Tree* tree) {

    ASSERTTREE (tree);

    bool Continue = false;

//    printf ("With who do you wanna play? \n");

    ChooseGamemode (tree);

    printf ("Want to play one more time?\n");

    if (YesOrNoRead ())
        Continue = true; 

    return Continue;

}

bool AddNewQuestion (Tree_node* question, Tree* tree) {

    ASSERTTREE (tree);

    assert (question);

    printf ("Unbelievable! Who is it then?\n");

    char* AnsBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* leaf = nullptr;

    if ((leaf = SearchElement (tree, AnsBuf)) != nullptr) {

        printf ("You what, why don't you what is %s?\n Ok, look riiiiiight here: %s is\n", AnsBuf, AnsBuf);
        ElemDef (tree, leaf);
        return true;

    } 

    printf ("\nAnd what the difference between %s and %s?\n", question -> data, AnsBuf);

    char* QueBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &QueBuf);

    if (!AddLeftLeaf (tree, question, AnsBuf))
        return false;

    if (!AddRightLeaf (tree, question, question -> data))
        return false;

    question -> data = QueBuf;

    printf ("%s %s added\n", QueBuf, AnsBuf);

    return true;

}

void LastQuestion (Tree_node* question, Tree* tree) {

    assert (question);
   
    if (YesOrNoRead ())
        printf ("I knew it!\n");

    else 
        AddNewQuestion (question, tree);        

    return;
    
}

void Ask (Tree_node* question, Tree* tree) {

    printf ("%s?\n", question -> data);

    if (question -> left == nullptr && question -> right == nullptr)
        LastQuestion (question, tree);

    else {

        if (question -> left == nullptr || question -> right == nullptr) {
            printf ("Database error: incomplete tree\n");
            return;
        }
    
        if (YesOrNoRead ())
            Ask (question -> left, tree);
        else
            Ask (question -> right, tree);

    }

    return;

}

void PrintNewDataBaseAndCloseDataBaseFile (Tree* tree, FILE** file) {
    
    ASSERTTREE (tree);
    assert (file);

    fclose (*file);
    *file = fopen (DataBaseFile, "w");

    assert (*file);

    PrintPreTree (*file, tree);

    fclose (*file); 

    return;    

}


