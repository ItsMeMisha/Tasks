#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "enum_commands.h"
#include <sys/stat.h>

char FileInDefault[] = "ProgIn.in";
char FileOutDefault[] = "MyProg.myexe";

struct label {

    char name[256];
    int place;

};

int NumOfSymbols (char* str, char symbol, int size);
int ArgumentsRead (char* Content, int numOfArgs, char* code, int* counter, label* labelsArr, int* labelsCounter);

int main (int argc, char* argv[]) {

	char FileInName[100] = "";
    strcpy (FileInName, FileInDefault);

	if (argc > 1)
		strcpy (FileInName, argv[1]);

    char FileOutName[100] = "";
    strcpy (FileOutName, FileOutDefault);

	if (argc > 2)
		strcpy (FileOutName, argv[2]);
			
	FILE* FileIn = fopen (FileInName, "r");

	assert (FileIn);

	struct stat FileInfo = {};

	struct stat* FileInfoPtr = &FileInfo;

	stat (FileInName, FileInfoPtr);

	char* FileContent = (char*) calloc (FileInfoPtr -> st_size, 1);
	char* FileContentStartPtr = FileContent;
	
	fread (FileContent, sizeof (char), FileInfoPtr -> st_size, FileIn);

	fclose (FileIn);

    int NumOfCommandsAndParameters = NumOfSymbols (FileContent, ' ', FileInfoPtr -> st_size) + NumOfSymbols (FileContent, '\0', FileInfoPtr -> st_size); 

    label* labelsArr = (label*) calloc (256, sizeof (label));
    int labelsCounter = 0;

	char StrBuf[100] = {};
    char* code = (char*) calloc (4 * NumOfCommandsAndParameters + 16, sizeof (char));
    
    code[0] = 'M';
    code[1] = 'V';
    code[2] = CommandsVersion;

	int ContentCount = 3;

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu)                                        \
                                                                                                \
        if (strcmp (StrBuf, #name) == 0) {                                                      \
                                                                                                \
            code[ContentCount] = CMD_##name;                                                    \
            ++ContentCount;                                                                     \
            FileContent += strlen (#name) + 1;                                                  \
                                                                                                \
            if (numOfArgs > 0)                                                                  \
                FileContent += ArgumentsRead (FileContent, numOfArgs, code, &ContentCount, labelsArr,&labelsCounter);  \
                                                                                                \
        } else                                                                             
                                                                                        
    for (int smth = 0; smth < 2; smth++ ) {
 
    ContentCount = 3;
    FileContent = FileContentStartPtr;

	while (FileContent - FileContentStartPtr < FileInfoPtr -> st_size) {

        while (isspace (*FileContent) && FileContent - FileContentStartPtr < FileInfoPtr -> st_size) 
    ++FileContent;
	
		sscanf (FileContent, "%s", StrBuf);

        #include "commands.h"

        if (*(StrBuf) == ':') {

            bool labelExist = false;

            for (int i = 0; i < labelsCounter; ++i) {
                
                if (strcmp (StrBuf, labelsArr[i].name) == 0) {

                    labelExist = true;
                    labelsArr[i].place = ContentCount;

                    break;

                }

            }

            if (!labelExist) {        

                strcpy (labelsArr[labelsCounter].name, StrBuf);
                labelsArr[labelsCounter].place = ContentCount;

                ++labelsCounter;

            }

            FileContent += strlen (StrBuf) + 1;

        }

        else {

            printf ("Syntax error: unexpected syntax (%s) \n", StrBuf);
            break;

       }

    }

    }

    #undef DEF_CMD
	
	FILE* FileOut = fopen (FileOutName, "wb");
	
	fwrite (code, sizeof (char), ContentCount, FileOut);

	fclose (FileOut);

    free (code);
	free (FileContentStartPtr);	
    free (labelsArr);

	return 0;

}

/*This function counts symbol in string
*
*   @param str - string
*   @param symbol
*   @param size - length of string
*
*   @return counter - number of symbols
*
*/

int NumOfSymbols (char* str, char symbol, int size) {

    assert (str);

    int counter = 0;

    for (int i = 0; i < size; ++i) 

        if (str[i] == symbol)
            ++counter;

    return counter;

}

/*This function reads numOfArgs arguments from Content and records them to code\n
*    If argument is a label then function records it to labalsArr and increases labelsCounter
*
*   @param Content
*   @param numOfArgs - number of arguments
*   @param code
*   @param counter - code counter
*   @param labelsArr - array of labels
*   @param labelsCounter - counter of labels
*
*   @return contentShift - shift of content pointer
*/

int ArgumentsRead (char* Content, int numOfArgs, char* code, int* counter, label* labelsArr, int* labelsCounter) {

    int contentShift = 0;

    char StrBuffer[30] = {};

    for (int i = 0; i < numOfArgs; ++i) {

        sscanf (Content + contentShift, "%s", StrBuffer);
        
        if ((isdigit (*StrBuffer)) || (*StrBuffer == '-') && (isdigit (*(StrBuffer + 1)))) {

            double NumBuffer = 0;
            sscanf (Content, "%lg", &NumBuffer);
            *((int*) (code + *counter)) = static_cast <int> (NumBuffer * Accuracy);
            *counter += sizeof (int);

            contentShift += strlen (StrBuffer) + 1;

        } 

        else if (*StrBuffer == ':') {

            bool labelExist = false;

            for (int i = 0; i < *labelsCounter; ++i) {
                
                if (strcmp (StrBuffer, labelsArr[i].name) == 0) {

                    labelExist = true;
                    *((int*) (code + *counter)) = labelsArr[i].place;
                    
                    break;

                }

            }

            if (!labelExist) {        

                strcpy (labelsArr[*labelsCounter].name, StrBuffer);
                ++(*labelsCounter); 

            }
            
            *counter += sizeof (int);

            contentShift += strlen (StrBuffer) + 1;

        } 

        else if (isalpha (StrBuffer[0]) && (StrBuffer[1] == 'x')) {

            code[*counter] = *StrBuffer - 'A';
            ++(*counter);
            contentShift += 3;

        }
            
    }

    return contentShift;

}
