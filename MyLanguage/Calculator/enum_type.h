#ifndef __NODETYPE__

    #define  __NODETYPE__

const int MaxComLength = 8;
const int MaxIdName    = 128;

typedef short varNum;

const char* func[] = {
    
    "", //0
    "+", //1
    "-", //2
    "*", //3
    "/", //4
    "sin",  //5
    "cos",  //6
    "tan",   //7
    "ctg", //8
    "^",//9
    "x",//10
    "0", //11
    "exp",  //12
    "ln",    //13
    "arcsin", //14
    "arccos", //15
    "arctan", //16

};

const int NumOfFunc = 17;

enum NodeType {

    TNum  = 0,
    TVar  = 1,
    TFunc = 2

}; 

#define NODE_TYPE(name, signature, num, prior, code) \
        type_##name = num,

enum FuncType {

    #include "types.h"

};

#undef NODE_TYPE

#endif
