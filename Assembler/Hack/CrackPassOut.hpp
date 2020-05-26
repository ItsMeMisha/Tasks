#ifndef __CRACK_PASS_OUT__
#define __CRACK_PASS_OUT__

#include <stdio.h>
#include <assert.h>
#include <cstring>
#include <sys/stat.h>

#define _DEBUG

#ifdef _DEBUG

    #define ASSERT( cond ) assert (cond)

#else
    
    #define ASSERT( cond ) 

#endif

namespace MaxsPass {

    enum ERRORS {

        OK         = 0,
        WRONG_FILE = 1
    };

    class File {

    public:
        File() = delete;
        File(const char* fileName);
        ~File();

        void changeJnz();
        void changeCall();
        void skipPass();

        ERRORS error = OK;
    
    private:
        const char* outputName = "CrackedProg.out";
        char* buffer = nullptr;
        int fileSize = 0;

        void readFile (const char* fileName);
        void writeToFile ();
        ERRORS checkFile ();

    };

    File::File(const char* fileName) {

        ASSERT (fileName);

        readFile (fileName);

        error = checkFile();
            
    }

    File::~File() {
        
        free(buffer);
    }

    void File::readFile (const char* fileName) {
 
        FILE* input = fopen (fileName, "rb");
        ASSERT (input);
        struct stat* fileInfo = (struct stat*) calloc (1, sizeof (struct stat));
        stat (fileName, fileInfo);

        fileSize = fileInfo->st_size;

        buffer = (char*) calloc (fileSize + 64, sizeof (char));
        fread (buffer, fileSize, sizeof (char), input);
        fclose (input);
    }

    ERRORS File::checkFile() {
        
        if (fileSize < 0x1CA)
            return WRONG_FILE;

        if (buffer[0] != 0x7F || buffer[1] != 0x45 || buffer[2] != 0x4C || buffer[3] != 0x46)
            return WRONG_FILE;

        return OK;
    }

    void File::changeJnz() {

        const unsigned char jnzOffset = 0xE9;
        const char jz = 0x74;
        const char jnz = 0x75;

        if (buffer[jnzOffset] == jz) {

            printf ("This program already has been hacked this way!\n");
            return;
        }

        buffer[jnzOffset] = jz;
        writeToFile();
        printf ("Done! Program has been hacked by replacing jnz\n");
    }

    void File::changeCall() {


    }

    void File::skipPass() {

        const char nop = 0x90;

        const unsigned char readFuncCallOffset = 0xB8;
        const size_t callLength = 5;

        const unsigned char jmpToBadOffset = 0xE9;
        const size_t jmpLength = 2;

        if (buffer[jmpToBadOffset] == nop || buffer[readFuncCallOffset] == nop) {

            printf ("This program already has been hacked this way!\n");
            return;
        }

        memset (buffer + readFuncCallOffset, nop, callLength);
        memset (buffer + jmpToBadOffset, nop, jmpLength);
        writeToFile ();
        printf ("Done! Program has been hacked by erasing of some commands\n");
    }

    void File::writeToFile () {

        FILE* output = fopen (outputName, "wb");
        ASSERT (output);

        fwrite (buffer, sizeof(char), fileSize, output);
        fclose (output);

    }

};

#endif
