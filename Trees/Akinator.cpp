#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Tree_t.h"

#define _DEBUG
#define VOICE

#ifdef VOICE
    
    #define say( text ) {\
        char* speech = (char*) calloc (strlen (text) + 50, sizeof (char)); \
        sprintf (speech, "espeak -p 0 -a 150 \"%s\"", text); \
        printf ("%s", text);\
        system (speech); \
        free (speech);  }

#else

    #define say( text ) printf ( "%s", text)

#endif

const int ShortAnsLen = 4;
const int MaxStrSize = 256;
const int MaxFileNameSize = 128;
const char DataBaseFile[MaxFileNameSize] = "AkinatorData.txt";

void StrToLower (char* str);
bool YesOrNoRead ();

Tree* CreateQuestionTreeAndOpenDataBaseFile (Tree* tree, FILE** file);

Tree_node*  CompareElems (Tree* tree, Tree_node* Elem1, Tree_node* Elem2, Element_t data1, Element_t data2);
void CompareMode (Tree* tree);
void Draw (Tree* tree);
void ElemDef (Tree* tree, Tree_node* node, Tree_node* RootBranch);
void DefinMode (Tree* tree);
void ChooseGamemode (Tree* tree);
bool Game (Tree* tree);
bool AddNewQuestion (Tree_node* question, Tree* tree);
void LastQuestion (Tree_node* question, Tree* tree);
void Ask (Tree_node* question, Tree* tree);

void PrintNewDataBaseAndCloseDataBaseFile (Tree* tree, FILE** file);

int main () { 

    system ("aplay MainSoundtrack.wav &");

    Tree tree = {};

    FILE* file = nullptr;
    if (CreateQuestionTreeAndOpenDataBaseFile (&tree, &file) == nullptr) {

        printf ("Database file error: cannot open file %s\n", DataBaseFile);
        return 1;

    }

    char SayingBuf[MaxStrSize] = "";

    sprintf (SayingBuf, "If you desturb me one more time, I will find you.\n Use 'y' and 'n' to say 'yes' or 'no'\n");
    say (SayingBuf);

    while (Game (&tree));

    sprintf (SayingBuf, "\nGoodbye, humanoid\n");
    say (SayingBuf);

    PrintNewDataBaseAndCloseDataBaseFile (&tree, &file);

    TreeDump (&tree);

    TreeDestruct (&tree);

    system ("killall aplay");

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

    if (strncmp (answer, "y", ShortAnsLen) == 0)
        return true;
    else if (strncmp (answer, "n", ShortAnsLen) == 0)
        return false;

    char SayingBuf[MaxStrSize] = "";
        
    sprintf (SayingBuf, "What the hell have you typed?! Type normally 'y' or 'n'!!!\n");
    say (SayingBuf);

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

Tree_node*  CompareElems (Tree* tree, Tree_node* Elem1, Tree_node* Elem2, Element_t data1, Element_t data2) {

    ASSERTTREE (tree);
    assert (Elem1);
    assert (Elem2);

    Tree_node* LastCommon = nullptr;

    if (!ParentsCheck (Elem1, tree))
        return LastCommon;

    if (!ParentsCheck (Elem2, tree))
        return LastCommon;

    char SayingBuf[MaxStrSize] = "";

    if (Elem1 -> parent == Elem2 -> parent) {

        if (Elem1 -> parent != tree -> root) {

            sprintf (SayingBuf, "%s and  %s both are ", data1, data2);
            say (SayingBuf);

            ElemDef (tree, Elem1 -> parent, tree -> root);

        }

        else {

            sprintf (SayingBuf, "%s and  %s have nothing in common ", data1, data2);
            say (SayingBuf);

        }


        sprintf (SayingBuf, " BUT ");
        say (SayingBuf);

        return Elem1 -> parent;

    }

    else {
        
        if (Elem1 -> parent == tree -> root || (Elem1 -> level < Elem2 -> level))
            LastCommon = CompareElems (tree, Elem1, Elem2 -> parent, data1, data2);

        else if (Elem2 -> parent == tree -> root || (Elem1 -> level < Elem2 -> level))
            LastCommon = CompareElems (tree, Elem1 -> parent, Elem2, data1, data2);

        else LastCommon = CompareElems (tree, Elem1 -> parent, Elem2 -> parent, data1, data2);

    }

    if (Elem1 -> data == data1 && Elem2 -> data == data2) {

        sprintf (SayingBuf, " %s is ", data1);
        say (SayingBuf);

        ElemDef (tree, Elem1, LastCommon);

        sprintf (SayingBuf, "\nAnd %s is ", data2);
        say (SayingBuf);

        ElemDef (tree, Elem2, LastCommon);


    }

    return LastCommon;

}

void CompareMode (Tree* tree) {

    ASSERTTREE (tree);

    char SayingBuf[MaxStrSize] = "";
    
    sprintf (SayingBuf, "What do you want me to compare?\n");
    say (SayingBuf);

    char* AnsBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* Elem1 = nullptr;

    if ((Elem1 = SearchElement (tree, AnsBuf)) == nullptr) {

        sprintf (SayingBuf, "I don't know what is it, go away!\n");
        say (SayingBuf);
        return;

    }

    sprintf (SayingBuf, "And???\n");
    say (SayingBuf);
 
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* Elem2 = nullptr;

    if ((Elem2 = SearchElement (tree, AnsBuf)) == nullptr) {

        sprintf (SayingBuf, "I don't know what is it, go away!\n");
        say (SayingBuf);
        return;

    }

    CompareElems (tree, Elem1, Elem2, Elem1 -> data, Elem2 -> data);
 
    return;

}

void Draw (Tree* tree) {

    ASSERTTREE (tree);

    FILE* fileout = fopen ("AkinatorTree.dot", "w");
    assert (fileout);

    DrawEasyTree (fileout, tree);

    fclose (fileout);
    system ("dot AkinatorTree.dot -T png -o AkinatorTree.png");

    char SayingBuf[MaxStrSize] = "";
    
    system ("fim -a AkinatorTree.png");
    sprintf (SayingBuf, "I draw the tree in AkinatorTree.png, as you can see, you could say \"thanks\" at least\n");
    say (SayingBuf);
    

    return;

}

void ElemDef (Tree* tree, Tree_node* node, Tree_node* RootBranch) {

    ASSERTTREE (tree);
    assert (node);
    if (RootBranch == nullptr)
        return;

    if (!ParentsCheck (node, tree))
        return;

    if (node -> parent != RootBranch && node -> parent != tree -> root)
        ElemDef (tree, node -> parent, RootBranch);

    char SayingBuf[MaxStrSize] = "";

    if (node == node -> parent -> left)
        sprintf (SayingBuf, "%s ", node -> parent -> data);
    
    if (node == node -> parent -> right)
        sprintf (SayingBuf, "not %s ", node -> parent -> data);

    say (SayingBuf);

    return;

}

void DefinMode (Tree* tree) {

    ASSERTTREE (tree);

    char SayingBuf[MaxStrSize] = "";

    sprintf (SayingBuf, "Ok, what do you want to know?\n");
    say (SayingBuf);

    char* AnsBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* leaf = nullptr;

    if ((leaf = SearchElement (tree, AnsBuf)) != nullptr) {

        sprintf (SayingBuf, "%s is ", AnsBuf);
        say (SayingBuf);

        ElemDef (tree, leaf, tree -> root);

    }

    else {

        sprintf (SayingBuf, "You've invented this word, you are cheating, please turn me off, I don't wanna talk with you anymore \n");
        say (SayingBuf);

    }

    return;

}

void ChooseGamemode (Tree* tree) {

    ASSERTTREE (tree);

    char SayingBuf[MaxStrSize] = "";
    
    sprintf (SayingBuf, "Come on, choose gamemode: \n");
    say (SayingBuf);

    sprintf (SayingBuf, "\t (1) Guessing \n");
    say (SayingBuf);

    sprintf (SayingBuf, "\t (2) Definitions \n");
    say (SayingBuf);

    sprintf (SayingBuf, "\t (3) Draw \n ");
    say (SayingBuf);

    sprintf (SayingBuf, "\t (4) Comparing  (Coming soon! Make a pre-order only for 300$ and play it erlier!)\n");
    say (SayingBuf);

    sprintf (SayingBuf, "\t (5) Exit \n");
    say (SayingBuf);
    

    char* answer = nullptr;
    scanf ("%ms", &answer);
    StrToLower (answer);

    if (strncmp (answer, "1", MaxStrSize) == 0) {

        bool Continue = true;

        while (Continue) {

            Continue = false;

            sprintf (SayingBuf, "Ok, let's start.\n");
            say (SayingBuf);

            Ask (tree -> root, tree);
            sprintf (SayingBuf, "Want to play one more time?\n");
            say (SayingBuf);

            if (YesOrNoRead ())
                Continue = true; 

        }

        return;

    }

    else if (strncmp (answer, "2", MaxStrSize) == 0) {

        bool Continue = true;

        while (Continue) {

            Continue = false;

            DefinMode (tree);
            sprintf (SayingBuf, "\nWant to learn one more word?\n");
            say (SayingBuf);

            if (YesOrNoRead ())
                Continue = true; 

        }


        return;
    }

    else if (strncmp (answer, "3", MaxStrSize) == 0) {

        Draw (tree);
        return;

    }

    else if (strncmp (answer, "4", MaxStrSize) == 0) {

        CompareMode (tree);
        return;

    }

    else if (strncmp (answer, "5", MaxStrSize) == 0)
        return;

    sprintf (SayingBuf, "What the hell have you typed?! Type normally!!!\n");
    say (SayingBuf);

    ChooseGamemode (tree);
    return;

}

bool Game (Tree* tree) {

    ASSERTTREE (tree);

    bool Continue = false;

//    printf ("With who do you wanna play? \n");

    ChooseGamemode (tree);

    char SayingBuf[MaxStrSize] = "";

    sprintf (SayingBuf, "\nWant to see main menu? (y). Or exit? (n)\n");
    say (SayingBuf);

    if (YesOrNoRead ())
        Continue = true; 

    return Continue;

}

bool AddNewQuestion (Tree_node* question, Tree* tree) {

    ASSERTTREE (tree);

    assert (question);

    char SayingBuf[MaxStrSize] = "";

    sprintf (SayingBuf, "Unbelievable! So, who is it?\n");
    say (SayingBuf);

    char* AnsBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* leaf = nullptr;


    if ((leaf = SearchElement (tree, AnsBuf)) != nullptr) {

        sprintf (SayingBuf, "Why?? Why don't you what is %s?\n Ok, look riiiiiight here: %s is\n", AnsBuf, AnsBuf);
        say (SayingBuf);

        ElemDef (tree, leaf, tree -> root);
        return true;

    } 

    sprintf (SayingBuf, "\nAnd what the difference between %s and %s?\n", question -> data, AnsBuf);
    say (SayingBuf);

    char* QueBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &QueBuf);

    if (!AddLeftLeaf (tree, question, AnsBuf))
        return false;

    if (!AddRightLeaf (tree, question, question -> data))
        return false;

    question -> data = QueBuf;

    sprintf (SayingBuf, "%s %s added\n", QueBuf, AnsBuf);
    say (SayingBuf);

    return true;

}

void LastQuestion (Tree_node* question, Tree* tree) {

    assert (question);
   
    if (YesOrNoRead ()) {

        char SayingBuf[MaxStrSize] = "";
        sprintf (SayingBuf, "I knew it!\n");
        say (SayingBuf);

    }

    else 
        AddNewQuestion (question, tree);        

    return;
    
}

void Ask (Tree_node* question, Tree* tree) {

    char SayingBuf[MaxStrSize] = "";

    sprintf (SayingBuf, "%s?\n", question -> data);
    say (SayingBuf);

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



