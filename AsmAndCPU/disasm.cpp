#include <stdio.h>
#include "enum_commands.h"
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>

const char FileInDefault[] = "MyProg.myexe";
const char FileOutDefault[] = "ProgText.out";

struct label {

  char name[256];
    int place;

}; 

CmdStruct CharToCmdStruct (const char cmd);
void PrintArgs (FILE* file, char** code, int numOfArgs);

int main (int argc, char* argv[]) {

   	char FileOutName[256] = "";
    strcpy (FileOutName, FileOutDefault);

	char FileInName[256] = "";
    strcpy (FileInName, FileInDefault);

	if (argc > 1)
		strcpy (FileInName, argv[1]);

	if (argc > 2)
		strcpy (FileOutName, argv[2]);
	
	FILE* FileIn = fopen (FileInName, "rb");

	assert (FileIn);

	struct stat FileInfo = {};

	struct stat* FileInfoPtr = &FileInfo;

	stat (FileInName, FileInfoPtr);

    char* content = (char*) calloc (FileInfoPtr -> st_size, sizeof (char));

    fread (content, 1, FileInfoPtr -> st_size, FileIn);

    if (FileInfoPtr -> st_size > 3)
        content += 3;

    label LabelsArr[256] = {};

    int LabelsNum = 0;

    const char* FileBegin = content - 3;

    fclose (FileIn);

    FILE* FileOut = fopen (FileOutName, "w");

    assert (FileOut);

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu)    \
                                                            \
        case CMD_##name: {                                  \
                                                            \
            fprintf (FileOut, "%s ", #name);                \
                                                            \
            if (numOfArgs > 0)                              \
                PrintArgs (FileOut, &content, numOfArgs);   \
            else {                                          \
                fprintf (FileOut, "\n");                    \
                ++content;                                  \
           }                                                \
                                                            \
                                                            \
            break; }                                        

    for (int i = 0; i < 2; ++i) {

        for (int j = 0; j < LabelsNum; ++j)
            if (LabelsArr[j].place == content - FileBegin)
                printf ("%s\n", LabelsArr[j].name);

        while (content - FileBegin < FileInfoPtr -> st_size) {

            switch ((*content & CmdNumMask) >> 3) {

                #include "commands.h"

                default: printf ("unknown command (%x) (%x)", (*content & CmdNumMask) >> 3, content - FileBegin);
                         return 1;

            }

        }

    }

    #undef DEF_CMD

    fclose (FileOut);

    return 0;

}

void PrintArgs (FILE* file, char** code, int numOfArgs) {

    assert (file);
    assert (code);

    CmdStruct CmdBuf = CharToCmdStruct (**code);

    if (CmdBuf.numofcmd  == CMD_push) {
        ++(*code);
        fprintf (file, "%lg ", (double) *((int*) *code) / Accuracy);
        *code += sizeof (int);
    
    } 

    else {

        for (int i = 0; i < numOfArgs; ++i) {

            if (CmdBuf.numofcmd >= CMD_jmp && CmdBuf.numofcmd <= CMD_call) {
                ++(*code);
                fprintf (file, "labelto%x ", *((int*) *code));
                *code += sizeof (int);
            }

            else if (CmdBuf.numofcmd == CMD_pop || CmdBuf.numofcmd == CMD_regpush) {
                ++(*code);
                fprintf (file, "%cx ", 'A' + **code);
                ++(*code);

            }

        }

    }
   fprintf (file, "\n"); 
}

CmdStruct CharToCmdStruct (const char cmd) {

    CmdStruct TmpCmd = {};
    TmpCmd.numofcmd = (cmd & CmdNumMask) >> 3;
    TmpCmd.numberparam = (cmd & NumberparamMask);
    TmpCmd.registerparam = (cmd & RegisterparamMask) >> 1;
    TmpCmd.ramparam = (cmd & RamparamMask) >> 2;

    return TmpCmd;

}
