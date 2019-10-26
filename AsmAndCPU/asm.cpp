#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "enum_commands.h"
#include <sys/stat.h>

#ifdef DEBUG

    #define ASSERT( cond ) assert (cond)

#else 

    #define ASSERT( cond ) assert (cond)

#endif

const int MaxLabels = 256;
const int MaxStrLen = 256;
const char FileInDefault[] = "ProgIn.in";
const char FileOutDefault[] = "MyProg.myexe";

struct label {

    char name[MaxStrLen];
    int place;

};

struct ManyLabels {

    label array[MaxLabels];
    int num;

};

void SkipSpace (char* Content, int* contentShift);
bool DoubleNumArgRead (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd);
bool IntNumArgRead (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd);
int LabelExist (ManyLabels lblArr, char* NewLabelName);
bool LabelArgRead (char* Content, int* contentShift, char* code, int* counter, ManyLabels* lblArr, CmdStruct* cmd);
bool RegArgRead (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd);
bool RamArgRead  (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd);

char CmdStructToChar (CmdStruct cmd);
int NumOfSymbols (char* str, char symbol, int size);
int ArgumentsRead (char* Content, int numOfArgs, char* code, int* counter, ManyLabels* lblArr, CmdStruct* cmd);

int main (int argc, char* argv[]) {

    char FileInName[MaxStrLen] = "";
    strcpy (FileInName, FileInDefault);

    if (argc > 1)
        strcpy (FileInName, argv[1]);

    char FileOutName[MaxStrLen] = "";
    strcpy (FileOutName, FileOutDefault);

    if (argc > 2)
        strcpy (FileOutName, argv[2]);
            
    FILE* FileIn = fopen (FileInName, "r");

    ASSERT (FileIn);

    struct stat FileInfo = {};

    struct stat* FileInfoPtr = &FileInfo;

    ASSERT (FileInfoPtr);

    stat (FileInName, FileInfoPtr);

    char* FileContent = (char*) calloc (FileInfoPtr -> st_size, sizeof (char));
    char* FileContentStartPtr = FileContent;

    ASSERT (FileContent);
    
    fread (FileContent, sizeof (char), FileInfoPtr -> st_size, FileIn);

    fclose (FileIn);

    int NumOfCommandsAndParameters = NumOfSymbols (FileContent, ' ', FileInfoPtr -> st_size) + NumOfSymbols (FileContent, '\0', FileInfoPtr -> st_size); 

    ManyLabels labelsArr = {};
    labelsArr.num = 0;

    char StrBuf[MaxStrLen] = "";
    char* code = (char*) calloc (sizeof (int) * NumOfCommandsAndParameters + 4*sizeof(int), sizeof (char));

    ASSERT (code);
    
    code[0] = 'M';
    code[1] = 'V';
    code[2] = CommandsVersion;

    CmdStruct CmdBuf = {};

    int ContentCount = 3;

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu)                                        \
                                                                                                \
        if (strcmp (StrBuf, #name) == 0) {                                                      \
                                                                                                \
            CmdBuf.numofcmd = CMD_##name;                                                       \
            ++ContentCount;                                                                     \
            FileContent += strlen (#name) + 1;                                                  \
            char* CurCode = code + ContentCount - 1;                                            \
                                                                                                \
            if (numOfArgs > 0) {                                                                \
                                                                                                \
                int contentShift = ArgumentsRead (FileContent, numOfArgs, code, &ContentCount, &labelsArr, labelsCounter, &CmdBuf);  \
                                                                                                \
                if (contentShift < 0) {                                                         \
                    printf ("Invalid parameters near %10s\n", FileContent);                     \
                    return 3;                                                                   \
                }                                                                               \
                                                                                                \
                FileContent += contentShift;                                                    \
                                                                                                \
            }                                                                                   \
                                                                                                \
            *CurCode = CmdStructToChar (CmdBuf);                                                \
            CmdBuf = {};                                                                        \
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

            for (int i = 0; i < labelsArr.num; ++i) {
                
                if (strcmp (StrBuf, labelsArr.array[i].name) == 0) {

                    labelExist = true;
                    labelsArr.array[i].place = ContentCount;

                    break;

                }

            }

            if (!labelExist) {        

                strcpy (labelsArr.array[labelsCounter].name, StrBuf);
                labelsArr.array[labelsCounter].place = ContentCount;

                ++labelsArr.num;

            }

            FileContent += strlen (StrBuf) + 1;

        }

        else {

            printf ("Syntax error: unexpected syntax (%s) \n", StrBuf);
            return 1;

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

/* This function turns CmdStruct to char
*
*   @param cmd as CmdStruct
*
*   @return NormalCommand - cmd as char 
*
*/

char CmdStructToChar (CmdStruct cmd) {

    char NormalCommand = 0;
    NormalCommand |= (cmd.numofcmd << 3);
    NormalCommand |= cmd.numberparam;
    NormalCommand |= (cmd.registerparam << 1);
    NormalCommand |= (cmd.ramparam << 2);  

    return NormalCommand;

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

/* This function skip space in line
*
*   @param Content - line
*   @param contentSift - shift from the begining of line
*
*/

void SkipSpace (char* Content, int* contentShift) {

    while (isspace (*(Content + *contentShift)))
        ++(*contentShift);

}

/* This function reads double number from Content and writes it in code
*
*   @return true if such argument exists
*
*/

bool DoubleNumArgRead (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd) {

    ASSERT (Content);
    ASSERT (code);
    ASSERT (counter);
    ASSERT (cmd);

    double NumBuffer = 0;
    int BufLen = 0;

    if (sscanf (Content + *contentShift, "%lg%n", &NumBuffer, &BufLen) > 0) {

        *((int*) (code + *counter)) = (int) (NumBuffer * Accuracy);
        *counter += sizeof (int);

        *contentShift += BufLen + 1;
        cmd -> numberparam = 1;
        
        return true;

    }

    else return false;

}

/* This function reads integer number from Content and writes it in code
*
*   @return true if such argument exists
*
*/

bool IntNumArgRead (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd) {
    
    ASSERT (Content);
    ASSERT (contentShift);
    ASSERT (code);
    ASSERT (counter);
    ASSERT (cmd);
    
    int NumBuffer = 0;
    int BufLen = 0;
    
    if (sscanf (Content + *contentShift, "%d%n", &NumBuffer, &BufLen) > 0) {

        *((int*) (code + *counter)) = NumBuffer * Accuracy;

        *counter += sizeof (int);
        *contentShift += BufLen;

        cmd -> numberparam = 1;

        return true;

    }

    else return false;

}

/* This function checks if label already exist
*
*   @return index of label if it exists
*
*/

int LabelExist (ManyLabels lblArr, char* NewLabelName) {

    ASSERT (NewLabelName);
    
    int index = 0;

    for (index; index < lblArr.num; ++index)
        if (strncmp (lblArr.array.name, NewLabelName, MaxStrLen))
            break;
    
    return index;

}

/* This Function reads label from content and writes it ot code
*
*   @return true if such argument exists
*
*/
    
bool LabelArgRead (char* Content, int* contentShift, char* code, int* counter, ManyLabels* lblArr, CmdStruct* cmd) {

    ASSERT (Content);
    ASSERT (contentShift);
    ASSERT (code);
    ASSERT (counter);
    ASSERT (lblArr);
    ASSERT (cmd);

    char StrBuf[MaxStrLen] = "";
    int BufLen = 0;

    sscanf (Content + *contentShift, "%s%n", StrBuf, &BufLen);

    if (StrBuf[0] == ':') { 

        const int index = LabelExist (*lblArr, StrBuf);

        if (lblArr -> num == index) {

            

        }     
//todo
    }
    
    else return false;

}

/* This Function reads register from content and writes it ot code
*
*   @return true if such argument exists
*
*/
 
bool RegArgRead (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd) {

    ASSERT (Content);
    ASSERT (contentShift);
    ASSERT (code);
    ASSERT (counter);
    ASSERT (cmd);

    const int BufLen = 2;

    if (isalpha (Content[contentShift]) && (Content[contentShift + 1] == 'x')) {

        code[*counter] = Content[contentShift] - 'A';
        ++(*counter);
        contentShift += BufLen;
            
        cmd -> registerparam = 1;

        return true;

    }

    else return false;

}

/* This Function reads ram argument from content and writes it ot code
*
*   @return true if such argument exists
*
*/
 
bool RamArgRead  (char* Content, int* contentShift, char* code, int* counter, CmdStruct* cmd) {

    ASSERT (Content);
    ASSERT (contentShift);
    ASSERT (code);
    ASSERT (counter);
    ASSERT (cmd);

//todo

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
*   @param cmd - pointer to CmdStruct struct
*
*   @return contentShift - shift of content pointer
*/

int ArgumentsRead (char* Content, int numOfArgs, char* code, int* counter, Manylabels* lblArr, CmdStruct* cmd) {

    ASSERT (Content);
    ASSERT (code);
    ASSERT (counter);
    ASSERT (labelsArr);
    ASSERT (labelsCounter);
    ASSERT (cmd);

    int contentShift = 0;

    for (int i = 0; i < numOfArgs; ++i) {

        if (!DoubleNumArdRead (Content, &contentSift, code, counter, cmd))

        if (!LabelArgRead (Content, &contentShift, code, counter, lblArr, cmd))
    
        if (!RegArgRead (Content, &contentShift, code, counter, cmd))

        if (!RamArgRead (Content, &contentShift, code, counter, cmd))

        return -1;
            
    }

    return contentShift;

}


