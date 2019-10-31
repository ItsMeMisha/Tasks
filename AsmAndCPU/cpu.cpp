#include "TXLin.h"
#include <stdio.h>
#include <math.h>
#include "enum_commands.h"
#include <sys/stat.h>
#include <malloc.h>
#include <assert.h>
#include "Stack_t.h"
#include <unistd.h>

#ifdef DEBUG

    #define ASSERT( cond ) assert (cond)

#else

    #define ASSERT( cond )

#endif

const int MaxStrLen = 256;
const int RAMSize = 4096;
const int VidMemSize = 1024;
const int CellSize = 100;
const int ClrNum = 11;
const int RegisterNum = 64;

const char FileInDefault[] = "MyProg.myexe";

void ReadCmdLineOptions (char* FileInName, int argc, char* argv[]);
char* ReadFile (char* FileInName, char* code);
bool Execution (char* cmd, Stack_t* Stack, int* RAM, int* regstr, int* VidMem, COLORREF* color);
void DrawCanv (int* VidMem,  COLORREF* color);
void VidReflex (int* VidMem);
void GenerateColors (COLORREF* color);

int main (int argc, char* argv[]) {

    Stack_t Stack = {};
    StackConstruct (&Stack);

    COLORREF color[ClrNum] = {};
 
    int RAM[RAMSize] = {};
    int* VidMem = RAM + RAMSize - VidMemSize;

    char FileInName[MaxStrLen] = "";
    ReadCmdLineOptions (FileInName, argc, argv);
 
    char* cmd = nullptr;
    cmd = ReadFile (FileInName, cmd);  
   
    int regstr[RegisterNum] = {};

    if (!Execution (cmd, &Stack, RAM, regstr, VidMem, color)) {

        printf ("Execution error\n");
        return 1;

    }
 
    free (cmd);

    return 0;

}           

void ReadCmdLineOptions (char* FileInName, int argc, char* argv[]) {

    ASSERT (FileInName);
    ASSERT (argv);

    strncpy (FileInName, FileInDefault, MaxStrLen);

    if (argc > 1) 
        strncpy (FileInName, argv[1], MaxStrLen);
    
    return;

}

char* ReadFile (char* FileInName, char* code) {

    ASSERT (FileInName);
   
    FILE* FileIn = fopen (FileInName, "rb");
    assert (FileIn);

    struct stat FileInfo = {};
    struct stat* FileInfoPtr = &FileInfo;
    stat (FileInName, FileInfoPtr);

    code = (char*) calloc (FileInfoPtr -> st_size, 1);

    fread (code, FileInfoPtr -> st_size, sizeof (char), FileIn);

    fclose (FileIn); 
     
    return code;

}

bool Execution (char* cmd, Stack_t* Stack, int* RAM, int* regstr, int* VidMem, COLORREF* color) {

    ASSERT (cmd);
    ASSERT (Stack);
    ASSERT (RAM);
    ASSERT (regstr);

    txCreateWindow (sqrt (VidMemSize), sqrt (VidMemSize));

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu) \
        case CMD_##name: { codeForCpu break; }              

    bool EndOfProgram = false;

    int current = 3;

    while (!EndOfProgram) {

        switch (((cmd[current] & CmdNumMask) >> 3) & 0x1f) {

            #include "commands.h"

            default: {

                printf ("unknown command (%x, %x)\n", ((cmd[current] & CmdNumMask) >> 3) & 0x1f, current);
                return false;
                break;
                
                DrawCanv (VidMem, color);
            }

        } 

    }

    return true;
    
    #undef DEF_CMD

}

void DrawCanv (int* VidMem, COLORREF* color) {

    ASSERT (VidMem);

    const int SqLen = (int) sqrt (VidMemSize);

    txBegin ();
    
    txSetFillColor (TX_WHITE);
    txClear ();
    int colorIndx = 0;

    for (int i = 0; i < SqLen; ++i)
        for (int j = 0; j < SqLen; ++j) {
            
            colorIndx = VidMem[i * SqLen + j] / Accuracy;
            txSetColor (color[colorIndx]);
            txSetFillColor (color[colorIndx]);

            txRectangle (CellSize * j, CellSize * i, (CellSize + 1) * j, (CellSize + 1) * i);

        }

    txEnd ();

    return;
}

void VidReflex (int* VidMem) {

    int SqLen = (int) sqrt (VidMemSize);
    for (int i = 0; i < SqLen; ++i)
        for (int j = 0; j < (SqLen / 2); ++j)
            VidMem[i * SqLen + SqLen - j - 1] = VidMem[i * SqLen + j];

}

void GenerateColors (COLORREF* color) {

    color[0] = TX_WHITE;
    color[1] = TX_LIGHTRED;
    color[2] = TX_RED;
    color[3] = TX_ORANGE;
    color[4] = TX_YELLOW;
    color[5] = TX_GREEN;
    color[6] = TX_CYAN;
    color[7] = TX_LIGHTBLUE;
    color[8] = TX_BLUE;
    color[9] = TX_PINK;
    color[10] = TX_BLACK;
    
    return;
}
