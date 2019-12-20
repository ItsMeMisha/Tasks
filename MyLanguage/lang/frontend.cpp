#include "grammar.h"
#include "middleend.h"
#include "MusicFrontEnd.h"

const int  MaxStrLen = 256;
const char FileInDefault[] = "test.muse";
const char FileOutDefault[] = "asmtest.mid";

void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, const char* argv[]);
void MakeMusic (Tree* tree);
void PrintToFile (char* fileName, Tree* tree);

int main (int argc, const char* argv[]) {

    char FileIn[MaxStrLen] = "";
    char FileOut[MaxStrLen] = "";

    ReadCmdLineOptions (FileIn, FileOut, argc, argv);

    Tree tree = {};

    TreeConstruct (&tree);
    GetTree (FileIn, &tree);
    OptimiseTree (&tree);

    PrintToFile (FileOut, &tree);
    MakeMusic (&tree);

    TreeDestruct (&tree);

    return 0;

}

void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, const char* argv[]) {

    strncpy (FileInName, FileInDefault, MaxStrLen);

    if (argc > 1)
        strncpy (FileInName, argv[1], MaxStrLen);

    strncpy (FileOutName, FileOutDefault, MaxStrLen);

    if (argc > 2)
        strncpy (FileOutName, argv[2], MaxStrLen);
 
    return;

}

void MakeMusic (Tree* tree) {

    assert (tree);

    FILE* out = fopen ("music.abc", "w");
    PrintMusic (out, tree);
    fclose (out);
    system ("abc2midi music.abc -o music.midi");

    return;

};

void PrintToFile (char* fileName, Tree* tree) {

    assert (fileName);
    assert (tree);

    FILE* out = fopen (fileName, "w");
    PrintPreTree (out, tree);
    fclose (out);
    TreeDump (tree);

    return;

}
