#include <stdio.h>
#include "enum_commands.h"
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>
#include "Labels.h"

#ifdef DEBUG 

    #define ASSERT( cond ) assert (code)

#else 

    #define ASSERT( cond )

#endif

const char FileInDefault[] = "MyProg.myexe";
const char FileOutDefault[] = "ProgText.out";

struct BuffersInfo {

    FILE* file;
    char* code;
    int codeShift;
    int codeSize;
    CmdStruct cmdBuf;

};

void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, char* argv[]);
char* ReadFile (const char* FileInName, int* fileSize, char* FileContent);
void PrepareToDisasm (BuffersInfo* BuffersPtr, ManyLabels* lblArr, int argc, char* argv[]);
 
CmdStruct CharToCmdStruct (const char cmd);

char ReadByteFromCode (BuffersInfo* BuffersPtr);
int ReadIntFromCode (BuffersInfo* BuffersPtr);

int PrintAsmTxt (BuffersInfo* BuffersPtr, ManyLabels* lblArr);

bool PrintIntNumArg (BuffersInfo* BuffersPtr);
bool PrintDoubleNumArg (BuffersInfo* BuffersPtr);
bool PrintRegArg (BuffersInfo* BuffersPtr);
bool PrintRamArg (BuffersInfo* BuffersPtr);
bool PrintLabelArg (BuffersInfo* BuffersPtr, ManyLabels* lblArr);
void PrintLabel (BuffersInfo* BuffersPtr, ManyLabels* lblArr);

bool PrintArgs (BuffersInfo* BuffersPtr, ManyLabels* lblArr, int numOfArgs);

int main (int argc, char* argv[]) {

    BuffersInfo Buffers = {};
    ManyLabels lblArr = {};

    PrepareToDisasm (&Buffers, &lblArr, argc, argv);    

    PrintAsmTxt (&Buffers, &lblArr);
    PrintAsmTxt (&Buffers, &lblArr);

    fclose (Buffers.file);
    free (Buffers.code - Buffers.codeShift);

    return 0;

}

/*! This function reads command line options
*
*   @param FileInName
*   @param FileOutName
*   @param argc
*   @param argv
*
*/

void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, char* argv[]) {

    strncpy (FileInName, FileInDefault, MaxStrLen);

    if (argc > 1)
        strncpy (FileInName, argv[1], MaxStrLen);

    strncpy (FileOutName, FileOutDefault, MaxStrLen);

    if (argc > 2)
        strncpy (FileOutName, argv[2], MaxStrLen);
 
    return;

}

/*! This function reads content from file and records it to FileContent\n
*       !!! NEED TO free (FileContent) !!!
*
*   @param FileInName
*   @param fileSize - here recording information about size of file
*   @param FileContent - !!! NEEDS TO FREE !!!
*
*   @return FileContent new address
*
*/

char* ReadFile (const char* FileInName, int* fileSize, char* FileContent) {

    ASSERT (FileInName);
    ASSERT (fileSize);

    FILE* FileIn = fopen (FileInName, "rb");
    ASSERT (FileIn);

    struct stat FileInfo = {};

    stat (FileInName, &FileInfo);

    *fileSize = FileInfo.st_size;

    FileContent = (char*) calloc (*fileSize, sizeof (char));
    
    ASSERT (FileContent);
    
    fread (FileContent, sizeof (char), *fileSize, FileIn);

    fclose (FileIn);

    return FileContent;

}

void PrepareToDisasm (BuffersInfo* BuffersPtr, ManyLabels* lblArr, int argc, char* argv[]) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);
    ASSERT (argv);
    ASSERT (FileOutName);

    char FileInName[MaxStrLen] = "";
    char FileOutName[MaxStrLen] = "";
    ReadCmdLineOptions (FileInName, FileOutName, argc, argv);

    char* code = nullptr;
    int fileSize = 0;
    code = ReadFile (FileInName, &fileSize, code);
    BuffersPtr -> code = code;
    BuffersPtr -> codeSize = fileSize;
 
    assert (fileSize > 3);
    BuffersPtr -> code += 3;
    BuffersPtr -> codeShift = 3;

    BuffersPtr -> file = fopen (FileOutName, "w");
    ASSERT (BuffersPtr -> file);

    lblArr -> num = 0;

    return;

}

char ReadByteFromCode (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);

    char Byte = *(BuffersPtr -> code);
    BuffersPtr -> code++;
    BuffersPtr -> codeShift++;

    return Byte;

}

int ReadIntFromCode (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);

    int Int = *((int*) (BuffersPtr -> code));
    BuffersPtr -> code += sizeof (int);
    BuffersPtr -> codeShift += sizeof (int);

    return Int;

}


#define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu)    \
                                                        \
    case CMD_##name: {                                  \
                                                        \
        fprintf (BuffersPtr -> file, "%s ", #name);     \
                                                        \
        if (numOfArgs > 0)                              \
            PrintArgs (BuffersPtr, lblArr, numOfArgs);  \
        else {                                          \
            BuffersPtr -> code++;                       \
            BuffersPtr -> codeShift++;                  \
        }                                               \
        fprintf (BuffersPtr -> file, "\n");             \
                                                        \
                                                        \
        break; }                                        


int PrintAsmTxt (BuffersInfo* BuffersPtr, ManyLabels* lblArr) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);

    fseek (BuffersPtr -> file, 0, SEEK_SET);

       while (BuffersPtr -> codeShift < BuffersPtr -> codeSize) {

            PrintLabel (BuffersPtr, lblArr);
 
            switch (((*(BuffersPtr -> code) & CmdNumMask) >> 3) & 0x1f) {

                #include "commands.h"

                default: printf ("unknown command (%x) (%x)", ((*(BuffersPtr -> code) & CmdNumMask) >> 3) & 0x1f, BuffersPtr -> codeShift);
                         return 1;

            }

        }

    BuffersPtr -> code += 3 - BuffersPtr -> codeShift;
    BuffersPtr -> codeShift = 3;

    return 0;
}

#undef DEF_CMD

bool PrintIntNumArg (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);

    if ((BuffersPtr -> cmdBuf.ramparam) && (BuffersPtr -> cmdBuf.numberparam)) {

        int Arg = ReadIntFromCode (BuffersPtr);
        fprintf (BuffersPtr -> file, "%d ", Arg / Accuracy);

        return true;

    }

    else return false;

}

bool PrintDoubleNumArg (BuffersInfo* BuffersPtr) {
    
    ASSERT (BuffersPtr);

    if ((BuffersPtr -> cmdBuf.ramparam == 0) && (BuffersPtr -> cmdBuf.numberparam)) {

        int Arg = ReadIntFromCode (BuffersPtr);
        fprintf (BuffersPtr -> file, "%lg ", (double) Arg / Accuracy);

        return true;

    }

    else return false;

}
bool PrintRegArg (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);
    
    if (BuffersPtr -> cmdBuf.registerparam) {

        char Arg = ReadByteFromCode (BuffersPtr);
        fprintf (BuffersPtr -> file, "%cx ", Arg + 'A');

        return true;

    }

    else return false;

}

bool PrintRamArg (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);

    if (BuffersPtr -> cmdBuf.ramparam) {

        fprintf (BuffersPtr -> file, "[");
        
        if (PrintIntNumArg (BuffersPtr))
            if (BuffersPtr -> cmdBuf.registerparam)
                fprintf (BuffersPtr -> file, "+ ");

        PrintRegArg (BuffersPtr);

        fprintf (BuffersPtr -> file, "] ");

        return true;

    }

    else return false;

}

bool PrintLabelArg (BuffersInfo* BuffersPtr, ManyLabels* lblArr) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);

    if ((BuffersPtr -> cmdBuf.numofcmd >= CMD_jmp) && (BuffersPtr -> cmdBuf.numofcmd <= CMD_call)) {

        char StrBuf[MaxStrLen] = {};
    
        int address = ReadIntFromCode (BuffersPtr);
        sprintf (StrBuf, ":LabelTo%d", address);

        int index = LabelExist (*lblArr, StrBuf);

        if (index == lblArr -> num)
            if (!AddNewLabel (lblArr, StrBuf, address))
                return false;
    
        fprintf (BuffersPtr -> file, "%s\n", StrBuf);

        return true;

    }

    else return false;

}

void PrintLabel (BuffersInfo* BuffersPtr, ManyLabels* lblArr) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);

    for (int i = 0; i < lblArr -> num; ++i)
        if (BuffersPtr -> codeShift == lblArr -> array[i].place) {
            
            fprintf (BuffersPtr -> file, ":LabelTo%d\n", lblArr -> array[i].place);
            break;

        }

    return;   

}


/* This function prints arguments of command in file
*
*   @param file
*   @param code - pointer to the array of binary code
*   @param numOfArgs - number of arguments
*   @param LabelsArr - array of labels
*   @param labelsNum - pointer to number of labels
*
*/

bool PrintArgs (BuffersInfo* BuffersPtr, ManyLabels* lblArr, int numOfArgs) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);

    BuffersPtr -> cmdBuf = CharToCmdStruct (*(BuffersPtr -> code));

    BuffersPtr -> code++;
    BuffersPtr -> codeShift++;

    for (int i = 0; i < numOfArgs; ++i) {

        if (!PrintRamArg (BuffersPtr))

        if (!PrintDoubleNumArg (BuffersPtr))

        if (!PrintRegArg (BuffersPtr))

        if (!PrintLabelArg (BuffersPtr, lblArr)) {        

            printf ("Invalid Argument\n");
            return false;

        }

    } 

    return true;

}

CmdStruct CharToCmdStruct (const char cmd) {

    CmdStruct TmpCmd = {};
    TmpCmd.numofcmd = ((cmd & CmdNumMask) >> 3) & 0x1f;
    TmpCmd.numberparam = (cmd & NumberparamMask);
    TmpCmd.registerparam = (cmd & RegisterparamMask) >> 1;
    TmpCmd.ramparam = (cmd & RamparamMask) >> 2;

    return TmpCmd;

}
