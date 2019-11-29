#ifndef __NODETYPE__

    #define  __NODETYPE__

const int MaxComLength = 8;

const char** func = {
    
    {"sin"},  //0
    {"cos"},  //1
    {"tg"},   //2
    {"ctg"},  //3
    {"exp"},  //4
    {"ln"}    //5

};    

#define NODE_TYPE(name, sign, num, prior, code) \
        type_##name = num,

enum NodeType {

    #include "types.h"

};

#undef NODE_TYPE

#endif
