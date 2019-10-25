#ifndef COMMANDS_H
#define COMMANDS_H

const char CmdNumMask = 0xf8;
const char NumberparamMask = 0x1;
const char RegisterparamMask = 0x2;
const char RamparamMask = 0x4;

struct CmdStruct {

    unsigned numofcmd :5;
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
