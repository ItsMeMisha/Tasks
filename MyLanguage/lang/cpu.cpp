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
const int RegisterNum = 64;

const char FileInDefault[] = "MyProg.myexe";

void ReadCmdLineOptions (char* FileInName, int argc, char* argv[]);
char* ReadFile (char* FileInName, char* code);
bool Execution (char* cmd, Stack_t* Stack, int* RAM, int* regstr);

int main (int argc, char* argv[]) {

    Stack_t Stack = {};
    StackConstruct (&Stack);

    int* RAM = (int*) calloc (sizeof (int), RAMSize);
 
    char FileInName[MaxStrLen] = "";
    ReadCmdLineOptions (FileInName, argc, argv);
 
    char* cmd = nullptr;
    cmd = ReadFile (FileInName, cmd);  
   
    int regstr[RegisterNum] = {};

    if (!Execution (cmd, &Stack, RAM, regstr)) {

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

bool Execution (char* cmd, Stack_t* Stack, int* RAM, int* regstr) {

    ASSERT (cmd);
    ASSERT (Stack);
    ASSERT (RAM);
    ASSERT (regstr);

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
                
            }

        } 

    }

    return true;
    
    #undef DEF_CMD

}


