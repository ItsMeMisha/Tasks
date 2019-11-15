#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Tree_t.h"

#define _DEBUG
#define VOICE

#ifdef VOICE
    
    #define say( ... ) {\
        char* speech = (char*) calloc (MaxStrSize + strlen (Espeak), sizeof (char)); \
        sprintf (speech, "%s", Espeak); \
        sprintf (speech + strlen (Espeak), __VA_ARGS__);\
        speech[strlen (speech)] = '"';\
        printf (__VA_ARGS__);\
        system (speech); \
        free (speech);  }

#else

    #define say( text ) printf ("%s", text)

#endif

const char Espeak[] = "espeak -p 0 -a 150 \"";
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

        printf ("\nDatabase file error: cannot open file %s\n", DataBaseFile);
        return 1;

    }

    say ("\n");

    say ("\n If you desturb me one more time, I will find you.\n Use 'y' or 'n' to say 'yes' or 'no'\n");

    while (Game (&tree));

    say ("\nGoodbye, humanoid\n");

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
        
    say ("What the hell have you typed?! Type normally 'y' or 'n'!!!\n");

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

    if (Elem1 -> parent == Elem2 -> parent) {

        if (Elem1 -> parent != tree -> root) {

            say ("%s and  %s both are ", data1, data2);

            ElemDef (tree, Elem1 -> parent, tree -> root);

        }

        else 
            say ("%s and  %s have nothing in common ", data1, data2);


        say (" BUT ");

        return Elem1 -> parent;

    }

    else {
        
        if (Elem1 -> level < Elem2 -> level)
            LastCommon = CompareElems (tree, Elem1, Elem2 -> parent, data1, data2);

        else if (Elem1 -> level > Elem2 -> level)
            LastCommon = CompareElems (tree, Elem1 -> parent, Elem2, data1, data2);

        else LastCommon = CompareElems (tree, Elem1 -> parent, Elem2 -> parent, data1, data2);

    }

    if (Elem1 -> data == data1 && Elem2 -> data == data2) {

        say (" %s is ", data1);

        ElemDef (tree, Elem1, LastCommon);

        say ("and %s is ", data2);

        ElemDef (tree, Elem2, LastCommon);

        say ("got it?\n");

    }

    return LastCommon;

}

void CompareMode (Tree* tree) {

    ASSERTTREE (tree);

    bool Continue = true;

    while (Continue) {
   
        say ("What do you want me to compare?\n");

        char* AnsBuf = nullptr;
        scanf ("%*[ \n]%m[^\n]", &AnsBuf);

        Tree_node* Elem1 = nullptr;

        if ((Elem1 = SearchElement (tree, AnsBuf)) == nullptr) {

            say ("I don't know what is it, go away!\n");
            continue;

        }
    
        say ("And???\n");
     
        scanf ("%*[ \n]%m[^\n]", &AnsBuf);
    
        Tree_node* Elem2 = nullptr;
    
        if ((Elem2 = SearchElement (tree, AnsBuf)) == nullptr) {
    
            say ("I don't know what is it, go away!\n");
            continue;

        }

        CompareElems (tree, Elem1, Elem2, Elem1 -> data, Elem2 -> data);

        say ("Want to compare one more time?\n");

        if (!YesOrNoRead ())
            Continue = false; 

    }
 
    return;

}

void Draw (Tree* tree) {

    ASSERTTREE (tree);

    FILE* fileout = fopen ("AkinatorTree.dot", "w");
    assert (fileout);

    DrawEasyTree (fileout, tree);

    fclose (fileout);
    system ("dot AkinatorTree.dot -T png -o AkinatorTree.png");
    
    system ("fim -a AkinatorTree.png");
    say ("I draw the tree in AkinatorTree.png, as you can see, you could say \"thanks\" at least\n");
  
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

    if (node == node -> parent -> left)
        say ("%s, ", node -> parent -> data);
    
    if (node == node -> parent -> right)
        say ("not %s, ", node -> parent -> data);

    return;

}

void DefinMode (Tree* tree) {

    ASSERTTREE (tree);

    say ("Ok, what do you want to know?\n");

    char* AnsBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* leaf = nullptr;

    if ((leaf = SearchElement (tree, AnsBuf)) != nullptr) {

        say ("%s is ", AnsBuf);

        ElemDef (tree, leaf, tree -> root);

    }

    else 
        say ("You've invented this word, you are cheating, please turn me off, I don't wanna talk with you anymore \n");

    return;

}

void ChooseGamemode (Tree* tree) {

    ASSERTTREE (tree);
    
    say ("Come on, choose gamemode: \n");

    say ("\t (1) Guessing \n");

    say ("\t (2) Definitions \n");

    say ("\t (3) Draw \n ");

    say ("\t (4) Comparing \n");

    say ("\t (5) Exit \n");
   

    char* answer = nullptr;
    scanf ("%ms", &answer);
    StrToLower (answer);

    if (strncmp (answer, "1", MaxStrSize) == 0) {

        bool Continue = true;

        while (Continue) {

            Continue = false;

            say ("Ok, let's start.\n");

            Ask (tree -> root, tree);

            say ("Want to play one more time?\n");

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
            say ("understandable? \nWant to learn one more word?\n");

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

    say ("What the hell have you typed?! Type normally!!!\n");

    ChooseGamemode (tree);
    return;

}

bool Game (Tree* tree) {

    ASSERTTREE (tree);

    bool Continue = false;

    ChooseGamemode (tree);

    say ("\nWant to see main menu? (y). Or exit? (n)\n");

    if (YesOrNoRead ())
        Continue = true; 

    return Continue;

}

bool AddNewQuestion (Tree_node* question, Tree* tree) {

    ASSERTTREE (tree);

    assert (question);

    say ("Unbelievable! So, who is it?\n");

    char* AnsBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &AnsBuf);

    Tree_node* leaf = nullptr;

    if ((leaf = SearchElement (tree, AnsBuf)) != nullptr) {

        say ("Why?? Why don't you what is %s?\n Ok, look riiiiiight here: %s is\n", AnsBuf, AnsBuf);

        ElemDef (tree, leaf, tree -> root);

        say ("now it's understandable? \n");

        return true;

    } 

    say ("\nAnd what the difference between %s and %s?\n", question -> data, AnsBuf);

    char* QueBuf = nullptr;
    scanf ("%*[ \n]%m[^\n]", &QueBuf);

    if (!AddLeftLeaf (tree, question, AnsBuf))
        return false;

    if (!AddRightLeaf (tree, question, question -> data))
        return false;

    question -> data = QueBuf;

    say ("%s %s added\n", QueBuf, AnsBuf);

    return true;

}

void LastQuestion (Tree_node* question, Tree* tree) {

    assert (question);
   
    if (YesOrNoRead ())
        say ("I knew it!\n")
    else 
        AddNewQuestion (question, tree);        

    return;
    
}

void Ask (Tree_node* question, Tree* tree) {

    say ("%s?\n", question -> data);

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



