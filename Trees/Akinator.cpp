#include <Tree_t.h>
#include <stdio.h>

#define _DEBUG

const int MaxFileNameSize = 128;
const char DataFile[MaxFileNameSize] = "AkinatorData.txt";

Tree* CreateQuestionTreeAndOpenDataBaseFile (Tree* tree, FILE** file);
bool Game                (Tree* tree);
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

    return Continue;

}

void Ask (Tree_node* question) {

    return;

}

void PrintNewDataBaseAndCloseDataBaseFile (Tree* tree, FILE** file) {

    PrintPreTree (*file, tree);

    fclose (*file); 

    return;    

}


