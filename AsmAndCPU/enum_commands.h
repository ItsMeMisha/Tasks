#ifndef COMMANDS_H
#define COMMANDS_H

char CmdNumMask = 0xf8;
char NumberparamMask = 0x1;
char RegisterparamMask = 0x2;
char RamparamMask = 0x4;

struct CmdStruct {

    int numofcmd :5;
    unsigned numberparam :1;
    unsigned registerparam :1;
    unsigned ramparam :1;

};

const char CommandsVersion = 3;
const int Accuracy = 10000;

#define DEF_CMD(name, cmdNum, numOfArg, codeForCpu) \
    CMD_##name = cmdNum,

enum commands {

    #include "commands.h"

};

#undef DEF_CMD

#endif
