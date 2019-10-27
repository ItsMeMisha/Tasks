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

struct BuffersInfo {

    char* Content;
    int contentSize;
    int contentShift;
    char* code;
    int counter;
    CmdStruct cmd;

};

struct label {

    char name[MaxStrLen];
    int place;

};

struct ManyLabels {

    label array[MaxLabels];
    int num;

};

void PrepareToAssemble (BuffersInfo* BuffersPtr, ManyLabels* lblArr, int argc, const char* argv[], char* FileOutName);
void DestroyBuffers (BuffersInfo* BuffersPtr);
void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, const char* argv[]);
char* ReadFile (const char* FileInName, int* fileSize, char* FileContent);
char* NewCode (const char* FileContent, const int FileSize, char* code);
int Assembler (BuffersInfo* BuffersPtr, ManyLabels* lblArr);
bool TryLabel (BuffersInfo* BuffersPtr, ManyLabels* lblArr, char* StrBuf);
bool DefCmd (BuffersInfo* BuffersPtr, ManyLabels* lblArr, const char* name, const int cmdNum, const int numOfArgs, char* StrBuf);

void AddIntToBinCode (char* code, int* counter, int value);
char CmdStructToChar (CmdStruct cmd);
int NumOfSymbols (const char* str, const char symbol, const int size);
void SkipSpace (char* Content, int* contentShift);

bool DoubleNumArgRead (BuffersInfo* BuffersPtr);
bool IntNumArgRead (BuffersInfo* BuffersPtr);

int LabelExist (ManyLabels lblArr, char* NewLabelName);
bool AddNewLabel (ManyLabels* lblArr, char* newName, int newPlace);
bool LabelArgRead (BuffersInfo* BuffersPtr, ManyLabels* lblArr);

bool RegArgRead (BuffersInfo* BuffersPtr);

bool ReadBeforePlusArg (BuffersInfo* BuffersPtr, char* regBuf);
bool ReadAfterPlusArg (BuffersInfo* BuffersPtr, char regBuf);
bool RamArgRead  (BuffersInfo* BuffersPtr);

int ArgumentsRead (BuffersInfo* BuffersPtr, int numOfArgs, ManyLabels* lblArr);

int main (int argc, const char* argv[]) {

    char FileOutName[MaxStrLen] = "";
    BuffersInfo Buffers = {};
    ManyLabels labelsArr = {};

    PrepareToAssemble (&Buffers, &labelsArr, argc, argv, FileOutName);

    Assembler (&Buffers, &labelsArr);
    Assembler (&Buffers, &labelsArr);     
    
    FILE* FileOut = fopen (FileOutName, "wb");
    
    fwrite (Buffers.code, sizeof (char), Buffers.counter, FileOut);

    fclose (FileOut); 

    DestroyBuffers (&Buffers);

    return 0;

}

void PrepareToAssemble (BuffersInfo* BuffersPtr, ManyLabels* lblArr, int argc, const char* argv[], char* FileOutName) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);
    ASSERT (FileOutName);

    char FileInName[MaxStrLen] = "";

    ReadCmdLineOptions (FileInName, FileOutName, argc, argv);

    int fileSize = 0;
    char* FileContent = nullptr; //!!!
    FileContent = ReadFile (FileInName, &fileSize, FileContent);
    BuffersPtr -> Content = FileContent;
    BuffersPtr -> contentShift = 0;
    BuffersPtr -> contentSize = fileSize;

    char* code = nullptr; //!!!
    code = NewCode (FileContent, fileSize, code);
    BuffersPtr -> code = code; 
    BuffersPtr -> counter = 3;

    BuffersPtr -> cmd = {};
    
    lblArr -> num = 0;

    return;

}

void DestroyBuffers (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);
    free (BuffersPtr -> code);
    free (BuffersPtr -> Content);
    BuffersPtr = nullptr;

    return;

}

/*! This function reads command line options
*
*   @param FileInName
*   @param FileOutName
*   @param argc
*   @param argv
*
*/

void ReadCmdLineOptions (char* FileInName, char* FileOutName, int argc, const char* argv[]) {

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

    FILE* FileIn = fopen (FileInName, "r");
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

/*! This function creats array for binary code
*
*   @param FileContent
*   @param FileSize
*   @param code !!! NEEDS TO FREE !!!
*
*   @return code - new address
*/

char* NewCode (const char* FileContent, const int FileSize, char* code) {

    ASSERT (FileContent);
    
    int NumOfCommandsAndParameters = NumOfSymbols (FileContent, ' ', FileSize) + NumOfSymbols (FileContent, '\0', FileSize); 

    code = (char*) calloc (sizeof (int) * NumOfCommandsAndParameters + 4*sizeof(int), sizeof (char));

    ASSERT (code);
    
    code[0] = 'M';
    code[1] = 'V';
    code[2] = CommandsVersion;

    return code;

}

/*! This function makes from Content and lblArr binary code
*
*   @param BuffersPtr
*   @param lblArr - array of labels
*
*   @return errcode
*
*/

int Assembler (BuffersInfo* BuffersPtr, ManyLabels* lblArr) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);

    BuffersPtr -> counter = 3;
    BuffersPtr -> contentShift = 0;
    char StrBuf[MaxStrLen] = "";

    #define DEF_CMD(name, cmdNum, numOfArgs, codeForCpu)                                        \
        if (!DefCmd (BuffersPtr, lblArr, #name, cmdNum, numOfArgs, StrBuf))                     \
            return 3;
 
    while (BuffersPtr -> contentShift < BuffersPtr -> contentSize) {

        SkipSpace (BuffersPtr -> Content, &(BuffersPtr -> contentShift));
    
        sscanf (BuffersPtr -> Content + BuffersPtr -> contentShift, "%s", StrBuf);

        #include "commands.h"

        if (!TryLabel (BuffersPtr, lblArr, StrBuf)) {

            printf ("Syntax error: unexpected syntax (%s) \n", StrBuf);
            return 1;

       }

    }

    #undef DEF_CMD

    return 0;

}

bool TryLabel (BuffersInfo* BuffersPtr, ManyLabels* lblArr, char* StrBuf) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);
    ASSERT (StrBuf);
 
    if (*(StrBuf) == ':') {

        int index = LabelExist (*lblArr, StrBuf); 

        if (index == lblArr -> num)
            if (!AddNewLabel (lblArr, StrBuf, BuffersPtr -> counter))
                return false;

        lblArr -> array[index].place = BuffersPtr -> counter;

        BuffersPtr -> contentShift += strlen (StrBuf);

    }

    return true;

}

bool DefCmd (BuffersInfo* BuffersPtr, ManyLabels* lblArr, const char* name, const int cmdNum, const int numOfArgs, char* StrBuf) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);
    ASSERT (name);

    if (strcmp (StrBuf, name) == 0) {                                                      
                                                                                               
        BuffersPtr -> cmd.numofcmd = cmdNum;                                                      
        ++(BuffersPtr -> counter);
        BuffersPtr -> contentShift += strlen (name) + 1;   
        char* CurCode = BuffersPtr -> code + BuffersPtr -> counter - 1;

        if (numOfArgs > 0) {                                                                
                                                                                              
            int NewContentShift = ArgumentsRead (BuffersPtr, numOfArgs, lblArr);               
                                                                                               
            if (NewContentShift < 0) {                                                         
                printf ("Invalid parameters near %10.10s\n", BuffersPtr -> Content + BuffersPtr -> contentShift - 2);
                return false; 
            }                                                                               
                                                                                                
        }                                                                                   
                                                                                                
        *CurCode = CmdStructToChar (BuffersPtr -> cmd); 
        BuffersPtr -> cmd = {};
                                                                                            
    }    

    return true;

} 

/*! This functions adds a value of int element to binary code
*
*   @param code
*   @param counter
*   @param value
*
*/

void AddIntToBinCode (char* code, int* counter, int value) {

    ASSERT (code);
    ASSERT (counter);

    *((int*) (code + *counter)) = value;
    *counter += sizeof (int);

    return;

}

/*! This function turns CmdStruct to char
*
*   @param cmd as CmdStruct
*
*   @return NormalCommand - cmd as char 
*
*/

char CmdStructToChar (CmdStruct cmd) {

    char NormalCommand = 0;
    NormalCommand |= cmd.numberparam;
    NormalCommand |= (cmd.registerparam << 1);
    NormalCommand |= (cmd.ramparam << 2);  
    NormalCommand |= (cmd.numofcmd << 3);

    return NormalCommand;

}

/*! This function counts symbol in string
*
*   @param str - string
*   @param symbol
*   @param size - length of string
*
*   @return counter - number of symbols
*
*/

int NumOfSymbols (const char* str, const char symbol, const int size) {

    assert (str);

    int counter = 0;

    for (int i = 0; i < size; ++i) 

        if (str[i] == symbol)
            ++counter;

    return counter;

}

/*! This function skip space in line
*
*   @param Content - line
*   @param contentSift - shift from the begining of line
*
*/

void SkipSpace (char* Content, int* contentShift) {

    while (isspace (*(Content + *contentShift)))
        ++(*contentShift);

}

/*! This function reads double number from Content and writes it in code
*
*   @return true if such argument exists
*
*/

bool DoubleNumArgRead (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);

    double NumBuffer = 0;
    int BufLen = 0;

    if (sscanf ((BuffersPtr -> Content) + (BuffersPtr -> contentShift), "%lg%n", &NumBuffer, &BufLen) > 0) {

        AddIntToBinCode (BuffersPtr -> code, &(BuffersPtr -> counter), (int) (NumBuffer * Accuracy));

        BuffersPtr -> contentShift += BufLen;
        BuffersPtr -> cmd.numberparam = 1;
        
        return true;

    }

    else return false;

}

/*! This function reads integer number from Content and writes it in code\n
*       !!! This function does NOT multiply a readed value by Accuray !!!
*
*   @return true if such argument exists
*
*/

bool IntNumArgRead (BuffersInfo* BuffersPtr) {
    
    ASSERT (BuffersPtr);
    
    int NumBuffer = 0;
    int BufLen = 0;
    
    if (sscanf ((BuffersPtr -> Content) + (BuffersPtr -> contentShift), "%d%n", &NumBuffer, &BufLen) > 0) {

        AddIntToBinCode (BuffersPtr -> code, &(BuffersPtr -> counter), NumBuffer * Accuracy);

        BuffersPtr -> contentShift += BufLen;

        BuffersPtr -> cmd.numberparam = 1;

        return true;

    }

    else return false;

}

/*! This function checks if label already exist
*
*   @return index of label if it exists and index = lblArr.num if it does not
*
*/

int LabelExist (ManyLabels lblArr, char* NewLabelName) {

    ASSERT (NewLabelName);
    
    int index = 0;

    for (index; index < lblArr.num; ++index) 
        if (strncmp (lblArr.array[index].name, NewLabelName, MaxStrLen) == 0)
            break;
    
    return index;

}

/*! This function adds new label with name newName and place newPlace to label's array
*
*   @param lblArr - array of labels
*   @param newName
*   @param newPlace
*
*   @return true if label was successfully added
*
*/

bool AddNewLabel (ManyLabels* lblArr, char* newName, int newPlace) {

    ASSERT (lblArr);
    ASSERT (newName);

    if (lblArr -> num < MaxLabels) {

        strncpy (lblArr -> array[lblArr -> num].name, newName, MaxStrLen);
        lblArr -> array[lblArr -> num].place = 0;
        ++(lblArr -> num);

        return true;

    }

    else {

        printf ("ERROR: too many unique labels\n");
        return false;

    }

}

/*! This Function reads label from content and writes it ot code
*
*   @return true if such argument exists
*
*/
    
bool LabelArgRead (BuffersInfo* BuffersPtr, ManyLabels* lblArr) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);

    char StrBuf[MaxStrLen] = "";
    int BufLen = 0;

    sscanf ((BuffersPtr -> Content) + (BuffersPtr -> contentShift), "%s%n", StrBuf, &BufLen);

    if (StrBuf[0] == ':') { 

        const int index = LabelExist (*lblArr, StrBuf);

        if (lblArr -> num == index)
            if (!AddNewLabel (lblArr, StrBuf, 0))
               return false;

        AddIntToBinCode (BuffersPtr -> code, &(BuffersPtr -> counter), lblArr -> array[index].place);

        BuffersPtr -> contentShift += BufLen;

        return true;

    }
    
    else return false;

}

/*! This Function reads register from content and writes it ot code
*
*   @return true if such argument exists
*
*/
 
bool RegArgRead (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);

    const int BufLen = 2;

    if (isalpha (BuffersPtr -> Content[BuffersPtr -> contentShift]) && (BuffersPtr -> Content[BuffersPtr -> contentShift + 1] == 'x')) {

        BuffersPtr -> code[BuffersPtr -> counter] = BuffersPtr -> Content[BuffersPtr -> contentShift] - 'A';
        ++(BuffersPtr -> counter); 
        BuffersPtr -> contentShift += BufLen;
            
        BuffersPtr -> cmd.registerparam = 1;

        return true;

    }

    else return false;

}

/*! This function reads argument before + for RamParam
*
*   @return true if successfully readed
*
*/

bool ReadBeforePlusArg (BuffersInfo* BuffersPtr, char* regBuf) {

    ASSERT (BuffersPtr);
    ASSERT (regBuf);

    if (IntNumArgRead (BuffersPtr));
    else if (RegArgRead (BuffersPtr)) {

        --(BuffersPtr -> counter);
        *regBuf = BuffersPtr -> code[BuffersPtr -> counter];
 
    }

    else return false;

    SkipSpace (BuffersPtr -> Content, &(BuffersPtr -> contentShift));

    return true;

}

/*! This function reads argument after + for RamParam
*
*   @return true if successfully readed
*
*/

bool ReadAfterPlusArg (BuffersInfo* BuffersPtr, char regBuf) {

    ASSERT (BuffersPtr);

    if (BuffersPtr -> Content[BuffersPtr -> contentShift] == '+') {

        ++(BuffersPtr -> contentShift);
        SkipSpace (BuffersPtr -> Content, &(BuffersPtr -> contentShift));
            
        if (regBuf == -1) {
            if (!RegArgRead (BuffersPtr))
                return false;
        }

        else if (!IntNumArgRead (BuffersPtr))
            return false;

        SkipSpace (BuffersPtr -> Content, &(BuffersPtr -> contentShift));

    }

    return true;

}

/*! This Function reads ram argument from content and writes it ot code
*
*   @return true if such argument exists
*
*/
 
bool RamArgRead  (BuffersInfo* BuffersPtr) {

    ASSERT (BuffersPtr);

    if (BuffersPtr -> Content[BuffersPtr -> contentShift] == '[') {

        ++(BuffersPtr -> contentShift);
        SkipSpace (BuffersPtr -> Content, &(BuffersPtr -> contentShift));

        char regBuf = -1;

        if (!ReadBeforePlusArg (BuffersPtr, &regBuf))
            return false;     
 
        if (!ReadAfterPlusArg (BuffersPtr, regBuf))
            return false;

        if (regBuf != -1) {

            *(BuffersPtr -> code + BuffersPtr -> counter) = regBuf;
            ++(BuffersPtr -> counter);
            
        }

        if (BuffersPtr -> Content[BuffersPtr -> contentShift] != ']')
            return false;
        else ++(BuffersPtr -> contentShift);

        BuffersPtr -> cmd.ramparam = 1;

        return true;

    }        

    else return false;       

}

/*! This function reads numOfArgs arguments from Content and records them to code\n
*    If argument is a label then function records it to labalsArr and increases labelsCounter
*
*   @param Content
*   @param numOfArgs - number of arguments
*   @param code
*   @param counter - code counter
*   @param lblsArr - array of labels
*   @param cmd - pointer to CmdStruct struct
*
*   @return contentShift - shift of content pointer
*/

int ArgumentsRead (BuffersInfo* BuffersPtr, int numOfArgs, ManyLabels* lblArr) {

    ASSERT (BuffersPtr);
    ASSERT (lblArr);

    for (int i = 0; i < numOfArgs; ++i) {

        SkipSpace (BuffersPtr -> Content, &(BuffersPtr -> contentShift));
 
        if (!DoubleNumArgRead (BuffersPtr))

        if (!LabelArgRead (BuffersPtr, lblArr))
    
        if (!RegArgRead (BuffersPtr))

        if (!RamArgRead (BuffersPtr))

        return -1;
           
    }

    return BuffersPtr -> contentShift;

}


