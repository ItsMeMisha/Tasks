#ifndef __NODETYPE__

    #define  __NODETYPE__

const int MaxComLength = 8;

#define NODE_TYPE(name, sign, num, code) \
        type_##name = num,

enum NodeType {

    #include "types.h"

};

#undef NODE_TYPE

#endif
