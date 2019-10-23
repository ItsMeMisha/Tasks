#include <stdio.h>
#include <math.h>
#include "enum_commands.h"
#include <sys/stat.h>
#include <malloc.h>
#include <assert.h>
#include "Stack_t.h"

int RAM[4096] = {};

char FileInDefault[] = "MyProg.myexe";

int main (int argc, char* argv[]) {

	char FileInName[100] = "";
    strcpy (FileInName, FileInDefault);

	if (argc > 1) 
		strcpy (FileInName, argv[1]);
	
	FILE* FileIn = fopen (FileInName, "rb");

	assert (FileIn);

	struct stat FileInfo = {};

	struct stat* FileInfoPtr = &FileInfo;

	stat (FileInName, FileInfoPtr);

	char* cmd = (char*) calloc (FileInfoPtr -> st_size, 1);

	fread (cmd, FileInfoPtr -> st_size, sizeof (char), FileIn);

    fclose (FileIn); 
	
	Stack_t Stack = {};

	StackConstruct (&Stack);

    int regstr[64] = {};

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu) \
        case CMD_##name: { codeForCpu break; }              \

    bool EndOfProgram = false;

    int current = 3;

	while (!EndOfProgram) {

        switch ((cmd[current] & CmdNumMask) >> 3) {

		    #include "commands.h"

            default: {

                printf ("unknown command (%d, %x)\n", (cmd[current] & CmdNumMask) >> 3, current);
                return 1;
                break;
            }

    	} 

    }

	return 0;

}			


