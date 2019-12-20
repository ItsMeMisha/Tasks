#include "backend.h"

const int  MaxStrLen = 256;
const char FileInDefault[] = "asmtest.mid";
const char FileOutDefault[] = "asmtest.asm";

void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, const char* argv[]);

int main (int argc, const char* argv[]) {

    char FileIn[MaxStrLen] = "";
    char FileOut[MaxStrLen] = "";

    ReadCmdLineOptions (FileIn, FileOut, argc, argv);

    Tree tree = {};
    TreeConstruct (&tree);
    FILE* input = fopen (FileIn, "r");
    ReadPreTree (input, &tree);
    fclose (input);
TreeDump (&tree);
    FILE* output = fopen (FileOut, "w");
    PrAsmCode (&tree, output);
    fclose (output);

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


