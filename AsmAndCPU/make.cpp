#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "enum_commands.h"

char FileInDefault[] = "ProgIn.in";
char FileMidDefault[] = "MyProg.myexe";
char FileOutDefault[] = "ProgOut.txt";

int main (int argc, char* argv[]) {

    char FileInName[100] = "";
    strcpy (FileInName, FileInDefault); 

    if (argc > 1)
        strcpy (FileInName, argv[1]);

    char FileMidName[100]  ="";
    strcpy (FileMidName, FileMidDefault);

    if (argc > 2)
        strcpy (FileMidDefault, argv[2]);

    char FileOutName[100] = "";
    strcpy (FileOutName, FileOutDefault);

    if (argc > 3)
        strcpy (FileOutName, argv[3]);

    char AsmCommand[100] = "";
    char CpuCommand[100] = "";

    sprintf (AsmCommand, "./asm.out %s %s", FileInName, FileMidName);
    sprintf (CpuCommand, "./cpu.out %s %s", FileMidName, FileOutName);

    system (AsmCommand);
    system (CpuCommand);   

} 
