#include <stdio.h>
#include "enum_commands.h"
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>

#ifdef DEBUG 

    #define ASSERT( cond ) assert (code)

#else 

    #define ASSERT( cond )

#endif

const char FileInDefault[] = "MyProg.myexe";
const char FileOutDefault[] = "ProgText.out";

struct label {

    char name[256];
    int place;

}; 

CmdStruct CharToCmdStruct (const char cmd);
void PrintArgs (FILE* file, char** code, int numOfArgs, label* LabelsArr, int* labelsNum);

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

	ASSERT (FileIn);

	struct stat FileInfo = {};

	struct stat* FileInfoPtr = &FileInfo;

    ASSERT (FileInfoPtr);

	stat (FileInName, FileInfoPtr);

    char* content = (char*) calloc (FileInfoPtr -> st_size, sizeof (char));

    ASSERT (content);

    fread (content, 1, FileInfoPtr -> st_size, FileIn);

    if (FileInfoPtr -> st_size > 3)
        content += 3;

    label LabelsArr[256] = {};

    int LabelsNum = 0;

    char* FileBegin = content - 3;

    fclose (FileIn);

    FILE* FileOut = fopen (FileOutName, "w");

    ASSERT (FileOut);

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu)    \
                                                            \
        case CMD_##name: {                                  \
                                                            \
            fprintf (FileOut, "%s ", #name);                \
                                                            \
            if (numOfArgs > 0)                              \
                PrintArgs (FileOut, &content, numOfArgs, LabelsArr, &LabelsNum);   \
            else {                                          \
                fprintf (FileOut, "\n");                    \
                ++content;                                  \
           }                                                \
                                                            \
                                                            \
            break; }                                        

    for (int i = 0; i < 2; ++i) {
        
       fseek (FileOut, 0, SEEK_SET);

       while (content - FileBegin < FileInfoPtr -> st_size) {

           for (int j = 0; j < LabelsNum; ++j)
               if (LabelsArr[j].place == content - FileBegin)
                   fprintf (FileOut, "%s\n", LabelsArr[j].name);
 
            switch (((*content & CmdNumMask) >> 3) & 0x1f) {

                #include "commands.h"

                default: printf ("unknown command (%x) (%x)", ((*content & CmdNumMask) >> 3) & 0x1f, content - FileBegin);
                         return 1;

            }

        }

        content = FileBegin + 3;

    }

    #undef DEF_CMD

    fclose (FileOut);
    free (FileBegin);

    return 0;

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

void PrintArgs (FILE* file, char** code, int numOfArgs, label* LabelsArr, int* labelsNum) {

    ASSERT (file);
    ASSERT (code);
    ASSERT (LabelsArr);
    ASSERT (labelsNum);

    CmdStruct CmdBuf = CharToCmdStruct (**code);

    if (CmdBuf.numofcmd >= CMD_jmp && CmdBuf.numofcmd <= CMD_call) {

        ++(*code);
        fprintf (file, ":labelto%x ", *((int*) *code));

        char LblName[256] = "";

        sprintf (LblName, ":labelto%x", *((int*) *code));
        
        bool labelExist = false;

        for (int i = 0; i < *labelsNum; ++i) {

            if (strcmp (LblName, LabelsArr[i].name) == 0) {

                labelExist = true;
                break;

            }                

        }

        if (!labelExist) {

            strcpy(LabelsArr[*labelsNum].name, LblName);
            LabelsArr[*labelsNum].place = **code;
            ++(*labelsNum);

        }

        *code += sizeof (int);
    }

    else for (int i = 0; i < numOfArgs; ++i) { 

        bool usesRam = false;

        if (CmdBuf.ramparam) {

            usesRam = true;
            fprintf (file, "[");

        }

        ++(*code);

        if (CmdBuf.numberparam) {

            fprintf (file, "%lg ", ((double) *((int*) *code) / Accuracy));
            *code += sizeof (int);

            if (usesRam && CmdBuf.registerparam) {

                fprintf (file, "+ ");
                fprintf (file, "%cx] ", 'A' + **code);
                ++(*code);                      

            }

            else if (usesRam)
                fprintf (file, "] "); 

        }

        if (CmdBuf.registerparam && (CmdBuf.numberparam == 0)) {
            
            fprintf (file, "%cx ", 'A' + **code);
            ++(*code);

            if (usesRam)
                fprintf (file, "]");

        }

    }
   
    fprintf (file, "\n"); 

    return;
}

CmdStruct CharToCmdStruct (const char cmd) {

    CmdStruct TmpCmd = {};
    TmpCmd.numofcmd = ((cmd & CmdNumMask) >> 3) & 0x1f;
    TmpCmd.numberparam = (cmd & NumberparamMask);
    TmpCmd.registerparam = (cmd & RegisterparamMask) >> 1;
    TmpCmd.ramparam = (cmd & RamparamMask) >> 2;

    return TmpCmd;

}
