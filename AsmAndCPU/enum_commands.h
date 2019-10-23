#ifndef COMMANDS_H
#define COMMANDS_H

char CmdNumMask = 0xf0;
char FirstparamMask = 0x1;
char SecondparamMask = 0x2;
char ThirdparamMask = 0x4;
char FourthparamMask = 0x8;

struct CmdStruct {

    int numofcmd :4;
    unsigned firstparam :1;
    unsigned secondparam :1;
    unsigned thirdparam :1;
    unsigned fourthparam :1;

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
