#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MaxStrLen = 256;
const char FileInDefault[] = "test.muse";
const char FileOutDefault[] = "asmtest.out";

void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, const char* argv[]);

int main (int argc, const char* argv[]) {

    char FileIn[MaxStrLen] = "";
    char FileOut[MaxStrLen] = "";

    char Buffer[2*MaxStrLen] = "";

    ReadCmdLineOptions (FileIn, FileOut, argc, argv);

    sprintf (Buffer, "./frontend.out %s asmtest.mid", FileIn);
    system (Buffer);
    system ("./backend.out asmtest.mid asmtest.asm");
    sprintf (Buffer, "./asm.out asmtest.asm %s", FileOut);
    system (Buffer);

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

