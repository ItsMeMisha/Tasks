#include <stdio.h>
#include "enum_commands.h"
#include <assert.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>

void PrintArgs (FILE* file, char** code, int numOfArgs);

int main (int argc, char* argv[]) {

   	char* FileOutName = "ProgText.out";
	char* FileInName = "MyProg.myexe";

	if (argc > 1)
		FileInName = argv[1];

	if (argc > 2)
		FileOutName = argv[2];
	
	FILE* FileIn = fopen (FileInName, "rb");

	assert (FileIn);

	struct stat FileInfo = {};

	struct stat* FileInfoPtr = &FileInfo;

	stat (FileInName, FileInfoPtr);

    char* content = (char*) calloc (FileInfoPtr -> st_size, sizeof (char));

    fread (content, 1, FileInfoPtr -> st_size, FileIn);

    if (FileInfoPtr -> st_size > 3)
        content += 3;

    const char* FileBegin = content - 3;

    fclose (FileIn);

    FILE* FileOut = fopen (FileOutName, "w");

    assert (FileOut);

    int lineCounter = 0;

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu)    \
                                                            \
        case CMD_##name: {                                  \
                                                            \
            fprintf (FileOut, "%d %s ", lineCounter, #name);\
                                                            \
            if (numOfArgs > 0)                              \
                PrintArgs (FileOut, &content, numOfArgs);   \
            else {                                          \
                fprintf (FileOut, "\n");                    \
                ++content;                                  \
           }                                                \
                                                            \
            ++lineCounter;                                  \
                                                            \
            break; }                                        


    while (content - FileBegin < FileInfoPtr -> st_size) {

        switch (*content) {

            #include "commands.h"

            default: printf ("unknown command (%d) (%x)", *content, content - FileBegin);
                     return 1;

        }

    }

    #undef DEF_CMD

    fclose (FileOut);

    return 0;

}

void PrintArgs (FILE* file, char** code, int numOfArgs) {

    assert (file);
    assert (code);

    if (**code == CMD_push) {
        ++(*code);
        fprintf (file, "%lg ", (double) *((int*) *code) / Accuracy);
        *code += sizeof (int);
    
    } 

    else {

        for (int i = 0; i < numOfArgs; ++i) {

            if (**code >= CMD_jmp && **code <= CMD_call) {
                ++(*code);
                fprintf (file, "labelto%x ", *((int*) *code));
                *code += sizeof (int);
            }

            else if (**code == CMD_pop || **code == CMD_regpush) {
                ++(*code);
                fprintf (file, "%cx ", 'A' + **code);
                ++(*code);

            }

        }

    }
   fprintf (file, "\n"); 
}
