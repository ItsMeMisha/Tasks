#include <Tree_t.h>
#include <stdio.h>

#define _DEBUG

const int MaxFileNameSize = 128;
const char DataFile[MaxFileNameSize] = "AkinatorData.txt";

bool YesOrNoRead ();

Tree* CreateQuestionTreeAndOpenDataBaseFile (Tree* tree, FILE** file);
bool Game (Tree* tree);
bool AddNewQuestion (Tree_node* question);
void LastQuestion (Tree_node* question);
void Ask (Tree_node* question);

void PrintNewDataBaseAndCloseDataBaseFile (Tree* tree, FILE** file);

int main () {

    Tree tree = {};

    FILE* file = nullptr;
    if (CreateQuestionTreeAndOpenDataBaseFile (&tree, &file) == nullptr) {

        printf ("Database file error: cannot open file %s\n", DataFile);
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

    Ask (tree -> root);

    printf ("Want to play one more time?\n");

    if (YesOrNoRead ())
        Continue = true; 

    return Continue;

}

bool AddNewQuestion (Tree_node* question) {

    assert (question);

    

}

void LastQuestion (Tree_node* question) {

    assert (question);
   
    if (YesOrNoRead ())
        printf ("I knew it!\n");

    else {

        printf ("Unbelievable! Who is it then?\n");
        AddNewQuestion (question);

    }        

    return;
    
}

void Ask (Tree_node* question) {

    printf ("%s?\n", question -> data);

    if (question -> left == nullptr && question -> == nullptr)
        LastQuestion (question);

    else

        if (question -> left == nullptr || question -> right == nullptr) {
            printf ("Database error: incomplete tree\n");
            return;
        }
    
        if (YesOrNoRead ())
            Ask (question -> left);
        else
            Ask (question -> right);

    return;

}

void PrintNewDataBaseAndCloseDataBaseFile (Tree* tree, FILE** file) {
    
    ASSERTTREE (tree);

    PrintPreTree (*file, tree);

    fclose (*file); 

    return;    

}


